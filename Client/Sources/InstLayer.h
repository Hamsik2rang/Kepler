#pragma once

#include <memory>

#include "Kepler.h"
#include "ParticleSystem.h"


class InstLayer : public kepler::Layer
{
private:
	kepler::OrthographicCamera m_camera;
	ParticleSystem m_particleEngine;
	float m_time;
	float m_lastCursorX;
	float m_lastCursorY;
	float m_screenWidth;
	float m_screenHeight;

public:
	InstLayer()
		: Layer("Instancing")
		, m_time{ 0.0f }
		, m_camera{}
	{

	}
	bool OnMouseMovedEvent(kepler::MouseMovedEvent& e);

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const float deltaTime) override;
	virtual void OnEvent(kepler::Event& e) override;
	virtual void OnRender() override;
};