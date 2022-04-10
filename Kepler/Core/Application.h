#pragma once

#include "Core/Base.h"
#include "Core/Window.h"

namespace kepler {
	// Window Application
	class KEPLER_API Application
	{
	protected:
		std::unique_ptr<IWindow> m_pWindow;

	public:
		Application();
		~Application();

		void OnEvent(Event& e);
		void Run();
	};
	// Defined in Client
	Application* CreateApplication();
}