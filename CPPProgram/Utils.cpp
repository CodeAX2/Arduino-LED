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

void cta::Utils::colorToHSV(sf::Color color, float* hsvArr) {

	float r = color.r / 255.0;
	float g = color.g / 255.0;
	float b = color.b / 255.0;

	// h, s, v = hue, saturation, value 
	double cmax = std::max(r, std::max(g, b)); // maximum of r, g, b 
	double cmin = std::min(r, std::min(g, b)); // minimum of r, g, b 
	double diff = cmax - cmin; // diff of cmax and cmin. 
	double h = -1, s = -1;

	// if cmax and cmax are equal then h = 0 
	if (cmax == cmin)
		h = 0;

	// if cmax equal r then compute h 
	else if (cmax == r)
		h = (int)(60 * ((g - b) / diff) + 360) % 360;

	// if cmax equal g then compute h 
	else if (cmax == g)
		h = (int)(60 * ((b - r) / diff) + 120) % 360;

	// if cmax equal b then compute h 
	else if (cmax == b)
		h = (int)(60 * ((r - g) / diff) + 240) % 360;

	// if cmax equal zero 
	if (cmax == 0)
		s = 0;
	else
		s = (diff / cmax) * 100;

	double v = cmax * 100;

	hsvArr[0] = h;
	hsvArr[1] = s;
	hsvArr[2] = v;

}

cta::ControllerApp* cta::Utils::app;