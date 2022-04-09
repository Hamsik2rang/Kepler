#include "kepch.h"

#include "WinAPI.h"
#include "Core/Log.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Core/Event/KeyEvent.hpp"
#include "Core/Event/ApplicationEvent.hpp"
#include "Core/Event/MouseEvent.hpp"

namespace kepler {
	
	extern HINSTANCE g_hInst = nullptr;
	extern int g_nCmdShow = SW_SHOWDEFAULT;

	ATOM kepler::RegisterWindowClass(const std::string& title, WindowsCallback callback)
	{
		WNDCLASSEXW wcex;

		std::wstring wTitle = kepler::StringToWString(title);

		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = callback;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = g_hInst;
		wcex.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_KEPLER));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_KEPLER);
		wcex.lpszClassName = wTitle.c_str();
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
		
		return ::RegisterClassExW(&wcex);
	}

	HWND kepler::InitInstance(const std::string& title, uint32_t width, uint32_t height, LONG_PTR pUserData)
	{
		auto wTitle = StringToWString(title);
		DWORD dwStyle = WS_OVERLAPPEDWINDOW;
		
		HWND hWnd = CreateWindowW(wTitle.c_str(),
			wTitle.c_str(), 
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 
			0, 
			(DWORD)width, 
			(DWORD)height, 
			nullptr, 
			nullptr, 
			g_hInst,
			0);

		if (!hWnd)
		{
			KEPLER_CORE_CRITICAL("Window can't instantiate");
			KEPLER_CORE_ASSERT(false, "");
		}

		ShowWindow(hWnd, g_nCmdShow);
		UpdateWindow(hWnd);
		
		HACCEL hAccelTable = LoadAccelerators(g_hInst, MAKEINTRESOURCE(IDC_KEPLER));

		::SetWindowLongPtr(hWnd, GWLP_USERDATA, pUserData);

		return hWnd;
	}

	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		kepler::WindowData& data = *reinterpret_cast<WindowData*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (msg)
		{
		case WM_COMMAND:
			{
				int wmId = LOWORD(wParam);
				switch (wmId)
				{
				case IDM_ABOUT:
					DialogBox(nullptr, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, msg, wParam, lParam);
				}
			}
			break;
		// Keyboard Events
		case WM_KEYDOWN:
			{
				KeyPressedEvent lastEvent(wParam, 0);
				data.eventCallback(lastEvent);
			}
			break;
		case WM_KEYUP:
			{
				KeyReleasedEvent lastEvent(wParam);
				data.eventCallback(lastEvent);
			}
			break;

		// Mouse Events
		case WM_MOUSEMOVE:
			{
				POINT cursorPos;
				::GetCursorPos(&cursorPos);
				MouseMovedEvent lastEvent(cursorPos.x, cursorPos.y);
				data.eventCallback(lastEvent);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				short z = GET_WHEEL_DELTA_WPARAM(wParam);
				MouseScrolledEvent lastEvent(z);
				data.eventCallback(lastEvent);
			}
			break;

		// Mouse Button Events
		case WM_LBUTTONDOWN:
			{
				MouseButtonPressedEvent lastEvent(-1);
				data.eventCallback(lastEvent);
			}
			break;
		case WM_LBUTTONUP:
			{
				MouseButtonReleasedEvent lastEvent(-1);
				data.eventCallback(lastEvent);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				MouseButtonPressedEvent lastEvent(1);
				data.eventCallback(lastEvent);
			}
			break;
		case WM_RBUTTONUP:
			{
				MouseButtonReleasedEvent lastEvent(1);
				data.eventCallback(lastEvent);
			}
			break;
		case WM_MBUTTONDOWN:
			{
				MouseButtonPressedEvent lastEvent(0);
				data.eventCallback(lastEvent);
			}
			break;
		case WM_MBUTTONUP:
			{
				MouseButtonReleasedEvent lastEvent(0);
				data.eventCallback(lastEvent);
			}
			break;

		// Application Events
		case WM_SETFOCUS:
			{

			}
			break;
		case WM_KILLFOCUS:
			{

			}
			break;
		case WM_SIZE:
			{
				
			}
			break;
		case WM_MOVE:
			{

			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}

	INT_PTR CALLBACK About(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (msg)
		{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}
		return (INT_PTR)FALSE;
	}

	std::wstring kepler::StringToWString(const std::string& s)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), 0, 0);
		auto buf = new wchar_t[len + 1];
		buf[len] = '\0';
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), buf, len + 1);
		auto ws = std::wstring(buf);
		delete[] buf;

		return ws;
	}

}