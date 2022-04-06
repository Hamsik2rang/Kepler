#include "kepch.h"

#include "Windows.h"

namespace kepler {
	HWND Windows::s_hMainWnd = nullptr;
	int Windows::s_windowCount = 0;

	IWindow* IWindow::Create(const WindowProps& props)
	{
		return new Windows(props);
	}

	Windows::Windows(const WindowProps& props)
	{
		Init(props);
	}

	Windows::~Windows()
	{
		Shutdown();
	}

	void Windows::Init(const WindowProps& props)
	{
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		KEPLER_CORE_INFO("Creating Window {0} ({1}, {2})", props.title, props.width, props.height);
		ATOM result = RegisterWindowClass(m_data.title, kepler::WndProc);
		KEPLER_CORE_INFO("Register window result is {0}", result);
		if (!result)
		{
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Initialize Main Window");
			throw;
		}
		m_hWnd = kepler::InitInstance(m_data.title, m_data.width, m_data.height);
		if (m_hWnd)
		{
			s_windowCount++;
		}
		else
		{
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Initialize Window Instance");
		}
		if (!s_hMainWnd)
		{
			s_hMainWnd = m_hWnd;
		}
		
		SetVSync(true);
	}

	void Windows::Shutdown()
	{
		// TODO:핸들 반납하기
	}

	void Windows::OnUpdate()
	{

	}

	void Windows::SetVSync(bool isEnabled)
	{
		if (isEnabled)
		{
			m_data.bVSync = true;
			// TODO: directx vsync 세팅
		}
		else
		{
			m_data.bVSync = false;
			// TODO: directx vsync 세팅
		}
	}
}
