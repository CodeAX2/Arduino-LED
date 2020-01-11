#include "ControllerApp.h"
#include "Utils.h"
#include "resource.h"
#include <strsafe.h>
#include <SFML/System.hpp>
#include "NoMode.h"
#include "AllStaticColorMode.h"

cta::ControllerApp::ControllerApp() : arduinoConnector("COM4") {

	mainFont.loadFromFile("Resources/OpenSans-Regular.ttf");


	arduinoConnector.connect();
	currentMode = new NoMode(this);
	currentMode->activate();

	new AllStaticColorMode(this);
	new NoMode(this);
	new NoMode(this);

}

bool cta::ControllerApp::claimMutex() {

	// Attempt to gain the mutex handle
	mutexHandle = CreateMutexA(0, FALSE, "Local\\ArduinoLEDControllerHofer");

	if (GetLastError() == ERROR_ALREADY_EXISTS) {

		// The mutex handle already exists, so there must be another instance
		HANDLE hFile = CreateFile(
			TEXT("\\\\.\\mailslot\\ArduinoLEDControllerHoferMailslot"),
			GENERIC_WRITE,
			FILE_SHARE_READ,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL
		);


		if (hFile == INVALID_HANDLE_VALUE) {
			printf("CreateFile failed with %d.\n", GetLastError());
		}

		// Send data to other instance to reopen its instance
		cta::Utils::writeSlot(hFile, L"Reopen Window");

		// Mutex was not claimed
		return false;
	}

	// No other running instance, so mutex claimed
	return true;
}

void cta::ControllerApp::begin() {
	claimMailSlotHandle();
	createEventWindow();
	createShellIconData();
	createMainWindow();
	beginEventRenderLoop();
}

sf::RenderWindow* cta::ControllerApp::getMainWindowPointer() {
	return &mainWindow;
}

void cta::ControllerApp::claimMailSlotHandle() {
	mailSlotHandle = CreateMailslot(TEXT("\\\\.\\mailslot\\ArduinoLEDControllerHoferMailslot"), 0, MAILSLOT_WAIT_FOREVER, (LPSECURITY_ATTRIBUTES)NULL);
}

void cta::ControllerApp::createEventWindow() {

	// Create the window
	WNDCLASS wc = { };

	wc.lpfnWndProc = cta::Utils::WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"Event Window";

	RegisterClass(&wc);

	eventWindowHandle = CreateWindowEx(
		0,
		L"Event Window",
		L"",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);

}

void cta::ControllerApp::createShellIconData() {

	// Create the icon
	icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(MAINICON));

	// Create the icon data for the tray
	NOTIFYICONDATAA shellIconData = {};
	shellIconData.cbSize = sizeof(shellIconData);
	shellIconData.hWnd = eventWindowHandle;
	shellIconData.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO | NIF_SHOWTIP;
	shellIconData.uID = 100;
	shellIconData.hIcon = icon;
	shellIconData.uCallbackMessage = WM_TRAY_ICON_MSG;
	shellIconData.szTip[sizeof(shellIconData.szTip) - 1] = '0';
	strncpy_s(shellIconData.szTip, "Arduino LED Confiurator", sizeof(shellIconData.szTip));

	// Add the icon to the tray
	Shell_NotifyIconA(NIM_ADD, &shellIconData);
}

void cta::ControllerApp::createMainWindow() {
	// Create the main SFML window

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 4;

	mainWindow.create(sf::VideoMode(1280, 720), "Arduino LED Confiurator", sf::Style::Close | sf::Style::Resize | sf::Style::Titlebar, settings);
	mainWindow.setFramerateLimit(120);

	// Set the window's icon
	SendMessage(mainWindow.getSystemHandle(), WM_SETICON, ICON_BIG, (LPARAM)icon);
}

void cta::ControllerApp::beginEventRenderLoop() {


	sf::Clock timer;
	sf::Time prev = timer.getElapsedTime();
	while (mainWindow.isOpen()) {

		// Handle events from the icon in the tray
		MSG msg;
		while (PeekMessage(&msg, eventWindowHandle, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Handle events relative to the main SFML window
		sf::Event e;
		while (mainWindow.pollEvent(e) && windowIsVisible) {
			if (e.type == sf::Event::Closed) {
				mainWindow.setVisible(false);
				windowIsVisible = false;
			} else if (e.type == sf::Event::Resized) {

				sf::Vector2u newSize = mainWindow.getSize();
				bool smallerThanMinimumSize = false;
				if (mainWindow.getSize().x < 600) {
					newSize.x = 600;
					smallerThanMinimumSize = true;
				}

				if (mainWindow.getSize().y < 300) {
					newSize.y = 300;
					smallerThanMinimumSize = true;
				}

				if (smallerThanMinimumSize)
					mainWindow.setSize(newSize);

				sf::Vector2f center;
				sf::Vector2f size;

				center.x = mainWindow.getSize().x / 2;
				center.y = mainWindow.getSize().y / 2;

				size = sf::Vector2f(mainWindow.getSize());

				currentMode->handleEvent(e);
				// The current mode can use the view
				// as the old window size

				sf::View newView(center, size);
				mainWindow.setView(newView);

			} else {
				currentMode->handleEvent(e);
			}
		}

		sf::Time now = timer.getElapsedTime();
		int dt = now.asMilliseconds() - prev.asMilliseconds();
		prev = now;

		tick(dt);

		// Draw graphics to the window
		if (windowIsVisible) {
			mainWindow.clear();
			draw(dt);
			mainWindow.display();
		}


		// Read data from the mail slot to see if we need
		// to reopen the window
		readMailSlot();

		if (!windowIsVisible) {
			sf::sleep(sf::milliseconds(1000 / 30));
		}

	}

	end();

}

void cta::ControllerApp::tick(int dt) {

	if (!arduinoConnector.isConnected()) {
		arduinoConnector.connect();
	}

	currentMode->tick(dt);

}

void cta::ControllerApp::draw(int dt) {

	currentMode->draw(dt);

}

void cta::ControllerApp::readMailSlot() {

	DWORD cbMessage, cMessage, cbRead;
	BOOL fResult;
	LPTSTR lpszBuffer;
	TCHAR achID[80] = {};
	DWORD cAllMessages;
	HANDLE hEvent;
	OVERLAPPED ov = {};

	cbMessage = cMessage = cbRead = 0;

	hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ExampleSlot"));

	if (NULL == hEvent)
		return;

	ov.Offset = 0;
	ov.OffsetHigh = 0;
	ov.hEvent = hEvent;

	fResult = GetMailslotInfo(
		mailSlotHandle,
		(LPDWORD)NULL,
		&cbMessage,
		&cMessage,
		(LPDWORD)NULL
	);

	if (!fResult) {
		printf("GetMailslotInfo failed with %d.\n", GetLastError());
		return;
	}

	if (cbMessage == MAILSLOT_NO_MESSAGE) {
		return;
	}

	cAllMessages = cMessage;

	while (cMessage != 0) {

		// Allocate memory for the message. 

		lpszBuffer = (LPTSTR)GlobalAlloc(
			GPTR,
			lstrlen((LPTSTR)achID) * sizeof(TCHAR) + cbMessage
		);

		if (NULL == lpszBuffer)
			return;

		lpszBuffer[0] = '\0';

		fResult = ReadFile(
			mailSlotHandle,
			lpszBuffer,
			cbMessage,
			&cbRead,
			&ov
		);

		if (!fResult) {
			printf("ReadFile failed with %d.\n", GetLastError());
			GlobalFree((HGLOBAL)lpszBuffer);
			return;
		}

		// Concatenate the message and the message-number string. 

		StringCbCat(lpszBuffer,
			lstrlen((LPTSTR)achID) * sizeof(TCHAR) + cbMessage,
			(LPTSTR)achID);

		if (wcscmp(lpszBuffer, L"Reopen Window") == 0) {
			// Reopen the window
			mainWindow.setVisible(true);
			windowIsVisible = true;
		}

		GlobalFree((HGLOBAL)lpszBuffer);

		fResult = GetMailslotInfo(
			mailSlotHandle,
			(LPDWORD)NULL,
			&cbMessage,
			&cMessage,
			(LPDWORD)NULL
		);

		if (!fResult) {
			printf("GetMailslotInfo failed (%d)\n", GetLastError());
			return;
		}
	}
	CloseHandle(hEvent);
	return;
}

void cta::ControllerApp::end() {
	Shell_NotifyIconA(NIM_DELETE, &shellIconData);
	CloseHandle(mutexHandle);
	arduinoConnector.disconnect();
}

void cta::ControllerApp::showMainWindow() {
	mainWindow.setVisible(true);
	windowIsVisible = true;
}

sf::Font& cta::ControllerApp::getMainFont() {
	return mainFont;
}

cta::ArduinoConnector* cta::ControllerApp::getArduinoConnector() {
	return &arduinoConnector;
}

void cta::ControllerApp::setCurrentLEDMode(cta::LEDMode* newMode) {
	currentMode->deActivate();
	currentMode = newMode;
	currentMode->activate();
}
