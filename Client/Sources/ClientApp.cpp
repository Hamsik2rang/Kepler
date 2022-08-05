#include "Kepler.h"
#include "ExampleLayer.h"
#include "InstLayer.h"

#include <Core/EntryPoint.h>

class Client : public kepler::Application
{
public:
	Client()
		: Application(kepler::eGraphicsAPI::DirectX11)
	{
		PushLayer(new ExampleLayer);
		PushOverlay(new kepler::RenderProfiler);
	}
	~Client()
	{

	}
};

kepler::Application* kepler::CreateApplication()
{
	return new Client();
}