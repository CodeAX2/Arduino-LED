#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>

#define WM_TRAY_ICON_MSG (WM_USER + 1)

namespace cta {
	class ControllerApp {

	public:
		ControllerApp();

	public:
		/*

		Attempts to claim the mutex. If there is another,
		instance of the program already running, then it
		sends a request to reopen the window in the other
		instance.

		Returns true if the mutex was claimed
		Returns false if the mutex was not claimed

		*/
		bool claimMutex();

		/*

		Begin the application, which sets up all
		required data and begins the drawing and
		event loop.

		*/
		void begin();

		/*

		Returns a pointer to the main window.

		*/
		sf::RenderWindow* getMainWindowPointer();


		/*
		
		Makes the main window visible.

		*/
		void showMainWindow();

	private:
		/*

		Claims the mail slot handle to recieve data
		from other instances of this program.

		*/
		void claimMailSlotHandle();

		/*

		Create a hidden window for dealing with
		events from the icon in the tray.

		*/
		void createEventWindow();


		/*

		Creates the icon data for the shell icon,
		and adds it to the tray.

		*/
		void createShellIconData();

		/*

		Creates the main window using SFML, sets its
		framerate and icon.

		*/
		void createMainWindow();

		/*

		Start the main event and render
		loop to actually make the program
		function/

		*/
		void beginEventRenderLoop();

		/*

		Called every frame. Draw graphics to
		the screen.

		*/
		void draw();

		/*

		Read any incomming data in the mail slot.
		If the mail slot recieves "Reopen Window", then
		show the main window.

		*/
		void readMailSlot();

		/*

		Called when the program ends, since the
		main window is no longer open. Cleans up
		resources, etc.

		*/
		void end();

	private:
		HANDLE mutexHandle;
		HANDLE mailSlotHandle;

		HWND eventWindowHandle;

		HICON icon;
		NOTIFYICONDATAA shellIconData;

		sf::RenderWindow mainWindow;
		bool windowIsVisible = true;

	};
}

