#pragma once

#include "Core/Base.h"


#include <Windows.h>
#include <string>

namespace kepler {
	
	using WindowsCallback = WNDPROC;

	ATOM RegisterWindowClass(const std::string& title, WindowsCallback callback);
	HWND InitInstance(const std::string& title, uint32_t width, uint32_t height, LONG_PTR pUserData);
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK About(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
	void ShowWindow(HWND hWnd);
}