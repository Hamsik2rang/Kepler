#include "kepch.h"

#include "Application.h"
#include "Core/Log.h"
#include "Core/Event/ApplicationEvent.hpp"
#include "Renderer/Renderer.h"
#include "Platform/DirectX11/DX11Context.h"
#include "Audio/Audio.h"

namespace kepler {

	Application* Application::s_pInstance = nullptr;

	Application::Application(eGraphicsAPI api)
	{
		IGraphicsAPI::SetAPI(api);
		m_pWindow = std::unique_ptr<IWindow>(IWindow::Create());

		// bind this->OnEvent
		m_pWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		s_pInstance = this;

		Renderer::Init();

		m_pImGuiLayer = std::make_unique<ImGuiLayer>();
		m_pImGuiLayer->OnAttach();

		m_bIsRunning = true;
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
		m_timer.Start();
		float lastTime = 0.0f;
		while (msg.message != WM_QUIT && m_bIsRunning)
		{
			// frame 60���� �����ϱ�
			float curTime = m_timer.Elapsed();
			float deltaTime = curTime - lastTime;
			if (deltaTime < 1.0f / 60.0f) 
			{
				continue; 
			}
			lastTime = curTime;

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			Renderer::Get()->ClearColor();
			// Update all layer(and overlay)s
			for (Layer* layer : m_layerStack)
			{
				layer->OnUpdate(deltaTime);
			}
			// Render all layer(and overlay)s
			for (Layer* layer : m_layerStack)
			{
				layer->OnRender();
			}

			// GUI Update
			m_pImGuiLayer->Begin();
			for (Layer* layer : m_layerStack)
			{
				layer->OnGUIRender();
			}

			// TODO: ���Ŀ� Editor Layer�� �����Ǿ� LayerStack�ȿ� ���� ���� 
			m_pImGuiLayer->OnGUIRender();

			m_pImGuiLayer->End();
			// Window Update
			m_pWindow->OnUpdate();
		}
	}
}
