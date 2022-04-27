#include "kepch.h"

#include "Application.h"
#include "Core/Log.h"
#include "Core/Event/ApplicationEvent.hpp"

namespace kepler{

	Application* Application::s_pInstance = nullptr;

	Application::Application()
	{
		m_pWindow = std::unique_ptr<IWindow>(IWindow::Create());
		
		// bind this->OnEvent
		m_pWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		s_pInstance = this;

		m_pGUILayer = std::make_unique<ImGuiLayer>();
		m_pGUILayer->OnAttach();
	}

	Application* Application::Get()
	{
		KEPLER_ASSERT(s_pInstance, "Application not constructed!");
		return s_pInstance;
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			// 가장 상위 레이어부터 시작해서 이벤트 처리
			// 각각의 레이어가 자신이 처리할 이벤트가 아니라면 그냥 무시합니다.
			(*(--it))->OnEvent(e);
			// 이벤트가 만약 임의의 레이어에 의해 처리되었다면 그 이벤트는 소멸한 것과 마찬가지이므로 더 이상 하위(후순위) 레이어에게 이벤트를 전달하면 안 됩니다.
			if (e.IsHandled())
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run()
	{
		KEPLER_CORE_INFO("Kepler is RUNNING...");

		MSG msg{};
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			m_pWindow->ClearRender();
			// Update all layer(and overlay)s
			for (Layer* layer : m_layerStack)
			{
				layer->OnUpdate();
			}
			// GUI Update
			m_pGUILayer->OnUpdate();
			// Window Update
			m_pWindow->OnUpdate();
		}
	}
}