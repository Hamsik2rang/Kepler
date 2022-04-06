#pragma once

#include "Core/Base.h"
#include "Core/Window.h"

namespace kepler {

	class KEPLER_API Application
	{
	protected:
		std::unique_ptr<IWindow> m_pWindow;
		bool m_bIsRunning = true;

	public:
		Application();
		~Application();

		void Run();
	};

	Application* CreateApplication();
}