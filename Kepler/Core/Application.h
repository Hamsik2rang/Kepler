#pragma once

#include "Core/Base.h"
#include "Core/Window.h"

#include "Core/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/GraphicsAPI.h"
#include "Utility/Timer.hpp"

namespace kepler {
	// Window Application
	class Application
	{
	private:
		static	Application*		s_pInstance;

		std::unique_ptr<IWindow>	m_pWindow;
		std::unique_ptr<ImGuiLayer> m_pImGuiLayer;
		LayerStack					m_layerStack;
		Timer						m_timer;
		
		bool m_bIsRunning;

	protected:
		Application(eGraphicsAPI api);

	public:
		~Application();

		static Application* Get();
		inline IWindow* GetWindow() { return m_pWindow.get(); }

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void Run();

		inline void Shutdown() { m_bIsRunning = false; }
	};
	// Defined in Client
	Application* CreateApplication();
}