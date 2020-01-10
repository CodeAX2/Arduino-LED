#include "Utils.h"
#include <stdio.h>

bool cta::Utils::writeSlot(HANDLE hSlot, LPCTSTR lpszMessage) {
	BOOL fResult;
	DWORD cbWritten;

	fResult = WriteFile(hSlot,
		lpszMessage,
		(DWORD)(lstrlen(lpszMessage) + 1) * sizeof(TCHAR),
		&cbWritten,
		(LPOVERLAPPED)NULL);

	if (!fResult) {
		printf("WriteFile failed with %d.\n", GetLastError());
		return false;
	}

	return true;
}

LRESULT CALLBACK cta::Utils::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_TRAY_ICON_MSG:

		switch (lParam) {
		case WM_LBUTTONDOWN:
			app->showMainWindow();
			ShowWindow(app->getMainWindowPointer()->getSystemHandle(), SW_RESTORE);
			break;
		case WM_RBUTTONDOWN:
			app->getMainWindowPointer()->close();
			break;
		}


		return 0;

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

cta::ControllerApp* cta::Utils::app;