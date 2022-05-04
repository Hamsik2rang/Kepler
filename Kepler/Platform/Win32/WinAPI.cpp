#include "kepch.h"

#include "Resource.h"
#include "framework.h"
#include "targetver.h"

#include "WinAPI.h"
#include "Core/Log.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Core/Event/KeyEvent.hpp"
#include "Core/Event/ApplicationEvent.hpp"
#include "Core/Event/MouseEvent.hpp"

#include "ImGui/ImGuiLayer.h"
#include "Core/Input.h"
#include "Utility\StringUtility.h"

namespace kepler {

	HINSTANCE g_hInst = nullptr;
	int		 g_nCmdShow = SW_SHOWDEFAULT;

	ATOM kepler::RegisterWindowClass(const std::string& title, WindowsCallback callback)
	{
		WNDCLASSEXW wcex;

		std::wstring wTitle = utility::StringToWstring(title);

		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_CLASSDC;
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
		auto wTitle = utility::StringToWstring(title);
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
		
		return hWnd;
	}

	void kepler::ShowWindow(HWND hWnd) 
	{ 

		::ShowWindow(hWnd, g_nCmdShow);
		::UpdateWindow(hWnd);

		HACCEL hAccelTable = LoadAccelerators(g_hInst, MAKEINTRESOURCE(IDC_KEPLER));
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

		if (ImGuiLayer::ImGuiEventHandler(hWnd, msg, wParam, lParam))
		{
			return true;
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
				int repeatCount = LOWORD(lParam);
				KeyPressedEvent lastEvent(static_cast<int>(wParam), repeatCount);
				Input::SetButtonDown(static_cast<kepler::MouseCode>(wParam));
				data->eventCallback(lastEvent);
			}
			break;
		case WM_KEYUP:
			{
				KeyReleasedEvent lastEvent(static_cast<int>(wParam));
				Input::SetButtonUp(static_cast<kepler::MouseCode>(wParam));
				
				data->eventCallback(lastEvent);
			}
			break;

		// Mouse Events
		case WM_MOUSEMOVE:
			{
				POINT cursorPos;
				::GetCursorPos(&cursorPos);
				::ScreenToClient(hWnd, &cursorPos); // Screen(모니터)상의 좌표를 실제 Client(엔진 창)상의 좌표로 바꿉니다.

				MouseMovedEvent lastEvent(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));
				data->eventCallback(lastEvent);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				short z = GET_WHEEL_DELTA_WPARAM(wParam);
				MouseScrolledEvent lastEvent(z, 0.0f);
				data->eventCallback(lastEvent);
			}
			break;

		// Mouse Button Events
		// 임시로 마우스 클릭 버튼을 좌/중간(휠)/우 에 따라 각각 0, 1, 2의 정수를 매핑했습니다.
		// MK_LBUTTON, MK_RBUTTON, MK_MBUTTON으로 매핑하는 것도 괜찮을 것 같은데.. 나중에 고민해 봅시다.
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			{
				int pressedButton = static_cast<int>(wParam);
				if (msg == WM_LBUTTONDOWN) { pressedButton = mouse::Left; }
				if (msg == WM_MBUTTONDOWN) { pressedButton = mouse::Middle; }
				if (msg == WM_RBUTTONDOWN) { pressedButton = mouse::Right; }
				MouseButtonPressedEvent lastEvent(pressedButton);
				Input::SetButtonDown(pressedButton);
				data->eventCallback(lastEvent);
			}
			break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			{
				int releasedButton = static_cast<int>(wParam);
				if (msg == WM_LBUTTONUP) { releasedButton = mouse::Left; }
				if (msg == WM_MBUTTONUP) { releasedButton = mouse::Middle; }
				if (msg == WM_RBUTTONUP) { releasedButton = mouse::Right; }
				MouseButtonReleasedEvent laseEvent(releasedButton);
				Input::SetButtonDown(releasedButton);
				data->eventCallback(laseEvent);
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
			// C++17 이전 버전에서 작동하지 않습니다.
			[[fallthrough]];
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
}