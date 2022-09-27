#pragma once

#include <memory>

#include "Kepler.h"
#include "Image.h"

class ResearchLayer : public kepler::Layer
{
private:
	kepler::OrthographicCamera m_camera;
	Image m_pImage;
	kepler::Entity* pEntity;
	kepler::Scene m_scene;


public:
	ResearchLayer()
		: Layer("Research")
	{}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnEvent(kepler::Event& e) override;
	virtual void OnRender() override;
	virtual void OnGUIRender() override;

};