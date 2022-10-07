#include "Kepler.h"
#include "ResearchLayer.h"
#include "InstLayer.h"

#include <Core/EntryPoint.h>

class Client : public kepler::Application
{
public:
	Client()
		: Application(kepler::EGraphicsAPI::DirectX11)
	{
		PushLayer(new InstLayer);
	}
	~Client()
	{

	}
};

kepler::Application* kepler::CreateApplication()
{
	return new Client();
}