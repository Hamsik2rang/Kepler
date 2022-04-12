#include "kepch.h"

#include "WinAPI.h"
#include "Core/Log.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Core/Event/KeyEvent.hpp"
#include "Core/Event/ApplicationEvent.hpp"
#include "Core/Event/MouseEvent.hpp"

namespace kepler {

	extern HINSTANCE g_hInst = nullptr;
	extern int		 g_nCmdShow = SW_SHOWDEFAULT;

	ATOM kepler::RegisterWindowClass(const std::string& title, WindowsCallback callback)
	{
		WNDCLASSEXW wcex;

		std::wstring wTitle = kepler::StringToWString(title);

		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = callback;
		wcex.cbClsExtra = 0;
		// 이벤트 핸들링을 위한 Userdata를 등록하기 위해 LONG_PTR 하나 만큼의 여분 메모리를 지정합니다.
		// 아래의 kepler::InitInstance 함수에서 ::SetWindowLongPtr 함수로 포인터가 등록됩니다.
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
		
		HWND hWnd = nullptr;
		hWnd = CreateWindowW(wTitle.c_str(),
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
		
		HACCEL hAccelTable = LoadAccelerators(g_hInst, MAKEINTRESOURCE(IDC_KEPLER));

		return hWnd;
	}

	void kepler::ShowWindow(HWND hWnd) 
	{ 
		::ShowWindow(hWnd, g_nCmdShow);
		::UpdateWindow(hWnd);
	}

	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		kepler::WindowData* data = reinterpret_cast<WindowData*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// CreateWindowW가 호출되면 생성 여부에 따라 WM_CREATE / WM_NCCREATE 메시지가 핸들링되는데, 이 시점에는 userdata의 포인터가 윈도우에 등록되기 이전입니다.
		// 따라서 data가 nullptr이면 메시지를 무시해야 합니다.
		// 추후 윈도우 생성 성공/실패에 대한 핸들링이 필요할 경우 아래 nullptr검사를 제외하고 switch에서 핸들링하면 됩니다.
		if (nullptr == data)
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

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
				data->eventCallback(lastEvent);
			}
			break;
		case WM_KEYUP:
			{
				KeyReleasedEvent lastEvent(wParam);
				data->eventCallback(lastEvent);
			}
			break;

		// Mouse Events
		case WM_MOUSEMOVE:
			{
				POINT cursorPos;
				::GetCursorPos(&cursorPos);
				::ScreenToClient(hWnd, &cursorPos); // Screen(모니터)상의 좌표를 실제 Client(엔진 창)상의 좌표로 바꿉니다.

				MouseMovedEvent lastEvent(cursorPos.x, cursorPos.y);
				data->eventCallback(lastEvent);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				short z = GET_WHEEL_DELTA_WPARAM(wParam);
				MouseScrolledEvent lastEvent(z);
				data->eventCallback(lastEvent);
			}
			break;

		// Mouse Button Events
		// 마우스 클릭 버튼을 좌/중간(휠)/우 에 따라 각각 -1, 0, 1의 정수를 매핑했습니다.
		// MK_LBUTTON, MK_RBUTTON, MK_MBUTTON으로 매핑하는 것도 괜찮을 것 같은데.. 나중에 고민해 봅시다.
		case WM_LBUTTONDOWN:
			{
				MouseButtonPressedEvent lastEvent(-1);
				data->eventCallback(lastEvent);
			}
			break;
		case WM_LBUTTONUP:
			{
				MouseButtonReleasedEvent lastEvent(-1);
				data->eventCallback(lastEvent);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				MouseButtonPressedEvent lastEvent(1);
				data->eventCallback(lastEvent);
			}
			break;
		case WM_RBUTTONUP:
			{
				MouseButtonReleasedEvent lastEvent(1);
				data->eventCallback(lastEvent);
			}
			break;
		case WM_MBUTTONDOWN:
			{
				MouseButtonPressedEvent lastEvent(0);
				data->eventCallback(lastEvent);
			}
			break;
		case WM_MBUTTONUP:
			{
				MouseButtonReleasedEvent lastEvent(0);
				data->eventCallback(lastEvent);
			}
			break;

		// Application Events
		case WM_SETFOCUS:
			{
				// TODO:나중에 윈도우 여러개 띄우게 되면(또는 GUI Layer 추가 시) 그때 구현합니다.
			}
			break;
		case WM_KILLFOCUS:
			{
				// TODO:나중에 윈도우 여러개 띄우게 되면(또는 GUI Layer 추가 시) 그때 구현합니다.
			}
			break;
		case WM_SIZE:
			{
				RECT rcWinRect{};
				::GetClientRect(hWnd, &rcWinRect);
				// Screen상의 좌표를 Client상의 좌표로 변환합니다.
				::ScreenToClient(hWnd, (POINT*)&rcWinRect.left);
				::ScreenToClient(hWnd, (POINT*)&rcWinRect.right);
				
				uint32_t width = rcWinRect.right - rcWinRect.left;
				uint32_t height = rcWinRect.bottom - rcWinRect.top;
				
				WindowResizeEvent lastEvent(width, height);
				data->width = width;
				data->height = height;
				data->eventCallback(lastEvent);
			}
			break;
		case WM_MOVE:
			{
				// D3D에서는 필요없는 것 같아서 일단 놔둡니다..
			}
			break;
		case WM_CLOSE:
			{
				WindowCloseEvent lastEvent;
				data->eventCallback(lastEvent);
			}
			// 추후에 WM_CLOSE 메시지 이후에 바로 윈도우를 Destroy하지 않고 무언가를 처리해야 한다면 fallthrough 로직을 없애고 break를 합시다!
			// C++17 이전 버전에서 작동하지 않습니다..
			[[fallthrough]]
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

	// std::string을 std::wstring으로 변환하는 함수
	std::wstring kepler::StringToWString(const std::string& s)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), 0, 0);
		// stl string을 c-style로 캐스팅할 경우 마지막에 '\0'이 추가되어야 하므로 
		// 문자형 단위 1만큼의 추가 공간이 필요합니다.(wchar문자열은 wchar_t 1개 만큼)
		auto buf = new wchar_t[len + sizeof(wchar_t)];	
		buf[len] = '\0';
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), buf, len + 1);
		// buf(LPWSTR)을 그대로 리턴해도 되지만 이 경우 함수 바깥에서 delete를 해 줘야만 하므로 mem leak 가능성이 있습니다.
		// 따라서 wstring을 리턴하는 함수로 설계했습니다.
		auto ws = std::wstring(buf);
		delete[] buf;

		return ws;
	}

}