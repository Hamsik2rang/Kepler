#pragma once


#include <Kepler.h>

class EditorLayer : public kepler::Layer
{
private:
	kepler::EditorCamera m_camera;

public:

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGUIRender() override;
	virtual void OnEvent(kepler::Event& e) override;
};