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
		//KEPLER_INFO("TestLayer Updated");
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
	{
		PushLayer(new TestLayer());
		PushOverlay(new kepler::ImGuiLayer());
	}
	~Client()
	{

	}
};

kepler::Application* kepler::CreateApplication()
{
	return new Client();
}