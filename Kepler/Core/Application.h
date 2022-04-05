#pragma once

#include "Core.h"
#include "Core/Window.h"


namespace kepler {

	class KEPLER_API Application
	{
	private:
		std::unique_ptr<IWindow> m_pWindow;

	public:
		Application();
		~Application();

		void Run();
	};

	Application* CreateApplication();
}