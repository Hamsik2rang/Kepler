#include "Kepler.h"
#include "ExampleLayer.h"
#include "InstLayer.h"

#include <Core/EntryPoint.h>

class Client : public kepler::Application
{
public:
	Client()
		: Application(kepler::eGraphicsAPI::DirectX11, kepler::WindowProperty(std::string("Test")))
	{
		PushLayer(new InstLayer);
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