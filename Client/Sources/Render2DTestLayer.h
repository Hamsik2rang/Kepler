#pragma once

#include "Kepler.h"

class Render2DTestLayer : public kepler::Layer
{
private:
	kepler::OrthographicCamera m_camera;

public:
	Render2DTestLayer()
		:Layer("Render2D Test")
	{
		
	}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const float deltaTime) override;
	virtual void OnEvent(kepler::Event& e) override;
};