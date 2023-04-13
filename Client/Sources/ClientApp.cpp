#include "InstLayer.h"
#include "Kepler.h"
#include "ResearchLayer.h"

#include <Core/EntryPoint.h>

class Client : public kepler::Application
{
public:
    Client()
        : Application(kepler::eGraphicsAPI::DirectX11)
    {
        PushLayer(new ResearchLayer);
    }
    ~Client()
    {

    }
};

kepler::Application* kepler::CreateApplication()
{
    return new Client();
}