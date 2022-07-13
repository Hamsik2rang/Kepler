#pragma once

#include "Kepler.h"
#include <memory>

class ExampleLayer : public kepler::Layer
{
private:
	std::shared_ptr<kepler::IVertexArray> m_pVertexArray;
	kepler::PerspectiveCamera m_camera;
	float m_time;

	bool OnKeyboardPressed(kepler::KeyPressedEvent& e);

public:
	ExampleLayer()
		: Layer("Example")
		, m_time{ 0.0f }
		, m_camera{ kepler::PerspectiveCamera{ kepler::Vec3f{ 0.0f, 1.0f, -6.0f }, kepler::Vec3f{ 0.0f, 1.0f, 0.0f },  10.0f, 45.0f, 16.0f / 9.0f, 0.1f, 1000.0f } }
	{

	}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const float deltaTime) override;
	virtual void OnEvent(kepler::Event& e) override;
};