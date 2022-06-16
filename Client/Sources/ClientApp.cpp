#include "GameLayer.h"


#include <Core/EntryPoint.h>

class Client : public kepler::Application
{
public:
	Client()
		: Application(kepler::eGraphicsAPI::DirectX11)
	{
		PushLayer(new GameLayer());
	}
	~Client()
	{

	}
};

kepler::Application* kepler::CreateApplication()
{
	return new Client();
}