#include "kepch.h"

#include "WindowsWindow.h"

namespace kepler {
	extern HINSTANCE	g_hInst;
	extern int			g_nCmdShow;

	HWND WindowsWindow::s_hMainWnd = nullptr;
	int WindowsWindow::s_windowCount = 0;

	IWindow* IWindow::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		KEPLER_CORE_INFO("Creating Window {0} ({1}, {2})", props.title, props.width, props.height);
		// 윈도우 클래스 등록
		ATOM result = RegisterWindowClass(m_data.title, kepler::WndProc);
		KEPLER_CORE_INFO("Register window result is {0}", result);
		if (!result)
		{
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Initialize Main Window");
			KEPLER_ASSERT(false, "Can't Initialize Main Window");
		}
		// 윈도우 클래스 초기화 및 이벤트 핸들링을 위한 유저 데이터 등록
		m_hWnd = kepler::InitInstance(m_data.title, m_data.width, m_data.height, reinterpret_cast<LONG_PTR>(&m_data));
		if (!m_hWnd)
		{
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Initialize Window Instance - {0} {1}", __FILE__, __LINE__);
			KEPLER_ASSERT(false, "Can't Initialize Window Instance - See CORE Log for more info.");
		}
		
		kepler::ShowWindow(m_hWnd);
		

		::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&m_data));
			
		s_windowCount++;
		if (!s_hMainWnd)
		{
			s_hMainWnd = m_hWnd;
		}
		
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		// TODO: 해제해야 할 윈도우 리소스가 있다면 이 곳에서 해제합니다.
	}

	void WindowsWindow::OnUpdate()
	{
		// TODO: Update Loop마다 해야할 작업들 작성
	}

	void WindowsWindow::SetVSync(bool isEnabled)
	{
		if (isEnabled)
		{
			// TODO: directx vsync 세팅
		}
		else
		{
			// TODO: directx vsync 세팅
		}
		m_data.bVSync = isEnabled;
	}
}
