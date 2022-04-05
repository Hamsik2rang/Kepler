#include "kepch.h"

#include "Windows.h"

namespace kepler {

	bool Windows::s_bIsInitialized = false;

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
		// TODO:윈도우 핸들 얻기

		if (!s_bIsInitialized)
		{
			// Initialize Window handle
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
			// directx vsync 세팅
		}
		else
		{
			m_data.bVSync = false;
			// directx vsync 세팅
		}
	}
}
