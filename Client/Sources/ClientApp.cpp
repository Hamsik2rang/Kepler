#include "InstLayer.h"
#include "Kepler.h"
#include "VulkanTestLayer.h"

#include <Core/EntryPoint.h>

class Client : public kepler::Application
{
public:
    Client()
        : Application(kepler::eGraphicsAPI::DirectX11)
    {
        PushLayer(new VulkanTestLayer);
    }
    ~Client()
    {

    }
};

kepler::Application* kepler::CreateApplication()
{
    return new Client();
}