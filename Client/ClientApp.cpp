#include <Kepler.h>


class TestLayer : public kepler::Layer
{
public:
	TestLayer()
		:Layer("Test")
	{
		
	}

	virtual void OnAttach() override
	{

	}

	virtual void OnDetach() override
	{

	}

	virtual void OnUpdate() override
	{
		// TODO: Update Loop마다 해야할 작업들 작성
		if (!kepler::Renderer::Get()->Render())
		{
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Rendering - {0} {1}", __FILE__, __LINE__);
			return;
		}
	}

	virtual void OnEvent(kepler::Event& e) override
	{
		KEPLER_TRACE("{0}", e);
	}
};

class Client : public kepler::Application
{
public:
	Client()
		:Application(kepler::eGraphicsAPI::DirectX11)
	{
		PushLayer(new TestLayer());
		//PushOverlay(new kepler::ImGuiLayer());
	}
	~Client()
	{

	}
};

kepler::Application* kepler::CreateApplication()
{
	return new Client();
}