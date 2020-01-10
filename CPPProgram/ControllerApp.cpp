#include "ControllerApp.h"
#include "Utils.h"
#include "resource.h"
#include <strsafe.h>

cta::ControllerApp::ControllerApp() {

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
	mainWindow.create(sf::VideoMode(1920, 720), "Arduino LED Confiurator");
	mainWindow.setFramerateLimit(120);

	// Set the window's icon
	SendMessage(mainWindow.getSystemHandle(), WM_SETICON, ICON_BIG, (LPARAM)icon);
}

void cta::ControllerApp::beginEventRenderLoop() {

	while (mainWindow.isOpen()) {

		// Handle events from the icon in the tray
		MSG msg;
		while (PeekMessage(&msg, eventWindowHandle, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Handle events relative to the main SFML window
		sf::Event e;
		while (mainWindow.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				mainWindow.setVisible(false);
				windowIsVisible = false;
			}
		}

		// Draw graphics to the window
		if (windowIsVisible) {
			mainWindow.clear();
			draw();
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

void cta::ControllerApp::draw() {

	sf::Image gradientImage;
	gradientImage.create(255, 255);
	for (int i = 0; i < gradientImage.getSize().x; i++) {
		for (int j = 0; j < gradientImage.getSize().y; j++) {

			sf::Color color;
			int imgSizeX = gradientImage.getSize().x;
			double scale = 255.0 * 6 / (double)imgSizeX;

			if (i <= imgSizeX / 6) {
				color.r = 255;
				color.g = i * scale;
				color.b = 0;
			} else if (i > imgSizeX / 6 && i <= imgSizeX / 3) {
				color.r = 510 - i * scale;
				color.g = 255;
				color.b = 0;
			} else if (i > imgSizeX / 3 && i <= imgSizeX / 2) {
				color.r = 0;
				color.g = 255;
				color.b = i * scale - 510;
			} else if (i > imgSizeX / 2 && i <= imgSizeX * 2 / 3) {
				color.r = 0;
				color.g = 1020 - i * scale;
				color.b = 255;
			} else if (i > imgSizeX * 2 / 3 && i <= imgSizeX * 5 / 6) {
				color.r = i * scale - 1020;
				color.g = 0;
				color.b = 255;
			} else {
				color.r = 255;
				color.g = 0;
				color.b = 1530 - i * scale;
			}

			color.r = color.r + (255 - color.r) * (double)j / gradientImage.getSize().y;
			color.g = color.g + (255 - color.g) * (double)j / gradientImage.getSize().y;
			color.b = color.b + (255 - color.b) * (double)j / gradientImage.getSize().y;

			gradientImage.setPixel(i, j, color);

		}

		sf::Texture t;
		t.loadFromImage(gradientImage);

		sf::RectangleShape shape;
		shape.setSize(sf::Vector2f(t.getSize()));
		shape.setTexture(&t);
		mainWindow.draw(shape);

	}

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
}

void cta::ControllerApp::showMainWindow() {
	mainWindow.setVisible(true);
	windowIsVisible = true;
}
