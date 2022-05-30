#pragma once

#include <Kepler.h>
#include <memory>
#include <vector>

class ExampleLayer: public kepler::Layer
{
private:
	std::shared_ptr<kepler::IVertexArray> m_pVertexArray[2];
	std::shared_ptr<kepler::ITexture2D> m_pTexture;

public:
	ExampleLayer()
		:Layer("Example")
	{

	}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const float deltaTime) override;
	virtual void OnEvent(kepler::Event& e) override;
};