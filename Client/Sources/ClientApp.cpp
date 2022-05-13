#include "ExampleLayer.h"


#include <Core/EntryPoint.h>


//class TestLayer : public kepler::Layer
//{
//public:
//	TestLayer()
//		:Layer("Test")
//	{
//		
//	}
//
//	virtual void OnAttach() override
//	{
//
//	}
//
//	virtual void OnDetach() override
//	{
//
//	}
//
//	virtual void OnUpdate() override
//	{
//
//	}
//
//	virtual void OnEvent(kepler::Event& e) override
//	{
//		KEPLER_TRACE("{0}", e);
//	}
//};

class Client : public kepler::Application
{
public:
	Client()
		:Application(kepler::eGraphicsAPI::DirectX11)
	{
		PushLayer(new ExampleLayer());
	}
	~Client()
	{

	}
};

kepler::Application* kepler::CreateApplication()
{
	return new Client();
}