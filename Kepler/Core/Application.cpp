#include "kepch.h"

#include "Application.h"
#include "Core/Log.h"
#include "Core/Event/ApplicationEvent.hpp"

namespace kepler{

	Application::Application()
	{
		m_pWindow = std::unique_ptr<IWindow>(IWindow::Create());
		
		// bind this->OnEvent
		m_pWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		KEPLER_CORE_TRACE("{0}", e);
	}

	void Application::Run()
	{
		printf("Hello!\n");
		KEPLER_CORE_INFO("Kepler is RUNNING...");
		MSG msg{};
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			m_pWindow->OnUpdate();
		}
	}
}