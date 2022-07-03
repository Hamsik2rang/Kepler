#include "kepch.h"

#include "WindowsWindow.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/Renderer.h"

namespace kepler {
	extern HINSTANCE	g_hInst;
	extern int			g_nCmdShow;

	IWindow* IWindow::Create(const WindowProperty& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProperty& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProperty& props)
	{
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;
		m_data.bVSync = true;

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
			KEPLER_ASSERT(false, "Can't Initialize Window Instance - See Core Log for more info.");
		}
		// D3D Device 생성
		IGraphicsContext::Create(&m_hWnd);
		if (!IGraphicsContext::Get()->Init(m_data))
		{
			IGraphicsContext::Get()->Cleanup();
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Initialize DirectX Device - {0} {1}", __FILE__, __LINE__);
			KEPLER_ASSERT(false, "Can't Initialize Window Instance - See Core Log for more info.");
		}

		kepler::ShowWindow(m_hWnd);
		::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&m_data));

		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		// TODO: 해제해야 할 윈도우 리소스가 있다면 이 곳에서 해제합니다.
		IGraphicsContext::Get()->Cleanup();
	}

	void WindowsWindow::OnUpdate()
	{
		// TODO: Update Loop마다 해야할 작업들 작성
		
		// swap back buffer and front buffer
		IGraphicsContext::Get()->SwapBuffer();
	}
}
