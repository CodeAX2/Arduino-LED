#pragma once
#include "ControllerApp.h"

namespace cta {

	class Utils {

	public:
		/*
		
		Write a message to a given mail slot handle.
		
		*/
		static bool writeSlot(HANDLE hSlot, LPCTSTR lpszMessage);

		/*
		
		Handle events of windows with this function set as
		their lpfnWndProc.

		*/
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		static cta::ControllerApp* app;

	};
}

