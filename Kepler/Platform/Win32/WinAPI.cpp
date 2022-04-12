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
		// �̺�Ʈ �ڵ鸵�� ���� Userdata�� ����ϱ� ���� LONG_PTR �ϳ� ��ŭ�� ���� �޸𸮸� �����մϴ�.
		// �Ʒ��� kepler::InitInstance �Լ����� ::SetWindowLongPtr �Լ��� �����Ͱ� ��ϵ˴ϴ�.
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
		// CreateWindowW�� ȣ��Ǹ� ���� ���ο� ���� WM_CREATE / WM_NCCREATE �޽����� �ڵ鸵�Ǵµ�, �� �������� userdata�� �����Ͱ� �����쿡 ��ϵǱ� �����Դϴ�.
		// ���� data�� nullptr�̸� �޽����� �����ؾ� �մϴ�.
		// ���� ������ ���� ����/���п� ���� �ڵ鸵�� �ʿ��� ��� �Ʒ� nullptr�˻縦 �����ϰ� switch���� �ڵ鸵�ϸ� �˴ϴ�.
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
				::ScreenToClient(hWnd, &cursorPos); // Screen(�����)���� ��ǥ�� ���� Client(���� â)���� ��ǥ�� �ٲߴϴ�.

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
		// ���콺 Ŭ�� ��ư�� ��/�߰�(��)/�� �� ���� ���� -1, 0, 1�� ������ �����߽��ϴ�.
		// MK_LBUTTON, MK_RBUTTON, MK_MBUTTON���� �����ϴ� �͵� ������ �� ������.. ���߿� ����� ���ô�.
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
				// TODO:���߿� ������ ������ ���� �Ǹ�(�Ǵ� GUI Layer �߰� ��) �׶� �����մϴ�.
			}
			break;
		case WM_KILLFOCUS:
			{
				// TODO:���߿� ������ ������ ���� �Ǹ�(�Ǵ� GUI Layer �߰� ��) �׶� �����մϴ�.
			}
			break;
		case WM_SIZE:
			{
				RECT rcWinRect{};
				::GetClientRect(hWnd, &rcWinRect);
				// Screen���� ��ǥ�� Client���� ��ǥ�� ��ȯ�մϴ�.
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
				// D3D������ �ʿ���� �� ���Ƽ� �ϴ� ���Ӵϴ�..
			}
			break;
		case WM_CLOSE:
			{
				WindowCloseEvent lastEvent;
				data->eventCallback(lastEvent);
			}
			// ���Ŀ� WM_CLOSE �޽��� ���Ŀ� �ٷ� �����츦 Destroy���� �ʰ� ���𰡸� ó���ؾ� �Ѵٸ� fallthrough ������ ���ְ� break�� �սô�!
			// C++17 ���� �������� �۵����� �ʽ��ϴ�..
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

	// std::string�� std::wstring���� ��ȯ�ϴ� �Լ�
	std::wstring kepler::StringToWString(const std::string& s)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), 0, 0);
		// stl string�� c-style�� ĳ������ ��� �������� '\0'�� �߰��Ǿ�� �ϹǷ� 
		// ������ ���� 1��ŭ�� �߰� ������ �ʿ��մϴ�.(wchar���ڿ��� wchar_t 1�� ��ŭ)
		auto buf = new wchar_t[len + sizeof(wchar_t)];	
		buf[len] = '\0';
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), buf, len + 1);
		// buf(LPWSTR)�� �״�� �����ص� ������ �� ��� �Լ� �ٱ����� delete�� �� ��߸� �ϹǷ� mem leak ���ɼ��� �ֽ��ϴ�.
		// ���� wstring�� �����ϴ� �Լ��� �����߽��ϴ�.
		auto ws = std::wstring(buf);
		delete[] buf;

		return ws;
	}

}