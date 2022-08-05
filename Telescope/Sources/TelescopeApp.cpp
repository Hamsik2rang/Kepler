#include <Kepler.h>

#include <Core/EntryPoint.h>

#include "EditorLayer.h"

class Editor : public kepler::Application
{
public:
	Editor()
		: Application(kepler::eGraphicsAPI::DirectX11)
	{
		PushLayer(new EditorLayer);
	}

	~Editor()
	{

	}
};

kepler::Application* kepler::CreateApplication()
{
	return new Editor;
}