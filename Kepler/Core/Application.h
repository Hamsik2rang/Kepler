#pragma once

#include "Core/Base.h"
#include "Core/Window.h"

#include "Core/LayerStack.h"

namespace kepler {
	// Window Application
	class KEPLER_API Application
	{
	protected:
		std::unique_ptr<IWindow> m_pWindow;
		LayerStack m_layerStack;

	public:
		Application();
		~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void Run();
	};
	// Defined in Client
	Application* CreateApplication();
}