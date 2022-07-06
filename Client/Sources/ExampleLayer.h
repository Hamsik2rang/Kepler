#pragma once

#include "Kepler.h"
#include <memory>

class ExampleLayer : public kepler::Layer
{
private:
	std::shared_ptr<kepler::IVertexArray> m_pVertexArray;
	float m_time;
public:
	ExampleLayer()
		: Layer("Example")
		, m_time{ 0.0f }
	{

	}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const float deltaTime) override;
	virtual void OnEvent(kepler::Event& e) override;
};