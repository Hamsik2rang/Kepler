#include "kepch.h"

#include "WinAPI.h"
#include "Core/Log.h"


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
		wcex.cbWndExtra = 0;
		wcex.hInstance = g_hInst;
		wcex.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_KEPLER));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_KEPLER);
		wcex.lpszClassName = wTitle.c_str();
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
		
		return ::RegisterClassExW(&wcex);
	}

	HWND kepler::InitInstance(const std::string& title, uint32_t width, uint32_t height)
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

		ShowWindow(hWnd, g_nCmdShow);	//TODO: nCmdShow 가변 변수로 받을 수 있게 변경해야 함. 
		UpdateWindow(hWnd);

		HACCEL hAccelTable = LoadAccelerators(g_hInst, MAKEINTRESOURCE(IDC_KEPLER));

		return hWnd;
	}

	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_COMMAND:
			{
				int wmId = LOWORD(wParam);
				// Parse the menu selections:
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
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code that uses hdc here...
				EndPaint(hWnd, &ps);
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