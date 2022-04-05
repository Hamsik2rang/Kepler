#include "kepch.h"

#include "Application.h"
#include "Core/Log.h"
#include "Core/Event/ApplicationEvent.hpp"

namespace kepler{

	Application::Application()
	{
		m_pWindow = std::unique_ptr<IWindow>(IWindow::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 700);
		if (e.IsInCategory(EventCategoryApplication))
		{
			KEPLER_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			KEPLER_TRACE(e);
		}
		while (true);
	}
}