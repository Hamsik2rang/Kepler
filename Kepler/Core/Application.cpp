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
			// ���� ���� ���̾���� �����ؼ� �̺�Ʈ ó��
			// ������ ���̾ �ڽ��� ó���� �̺�Ʈ�� �ƴ϶�� �׳� �����մϴ�.
			(*(--it))->OnEvent(e);
			// �̺�Ʈ�� ���� ������ ���̾ ���� ó���Ǿ��ٸ� �� �̺�Ʈ�� �Ҹ��� �Ͱ� ���������̹Ƿ� �� �̻� ����(�ļ���) ���̾�� �̺�Ʈ�� �����ϸ� �� �˴ϴ�.
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