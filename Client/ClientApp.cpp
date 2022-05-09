#include <Kepler.h>
#include "TestLayer.h"


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