#include "Render2DTestLayer.h"


#include <Core/EntryPoint.h>

class Client : public kepler::Application
{
public:
	Client()
		:Application(kepler::eGraphicsAPI::DirectX11)
	{
		PushLayer(new Render2DTestLayer());
	}
	~Client()
	{

	}
};

kepler::Application* kepler::CreateApplication()
{
	return new Client();
}