#pragma once


#include <Kepler.h>

class EditorLayer : public kepler::Layer
{
private:
	kepler::EditorCamera m_camera;

	kepler::Entity* m_pCubeEntity;
	kepler::Scene m_scene;

	float m_time;
	bool m_bIsSceneFocuced;

	uint32_t m_viewportWidth;
	uint32_t m_viewportHeight;
	uint32_t m_lastViewportWidth;
	uint32_t m_lastViewportHeight;
public:

	EditorLayer()
		: Layer("Editor")
		, m_camera{ kepler::EditorCamera{ kepler::Vec3f{ 0.0f, 1.0f, -6.0f }, kepler::Vec3f{ 0.0f, 1.0f, 0.0f }, 10.0f, 45.0f, 16.0f / 9.0f, 0.1f, 1000.0f } }
		, m_time{ 0.0f }
		, m_bIsSceneFocuced{false}
		, m_viewportWidth{ 0 }
		, m_viewportHeight{ 0 }
		, m_lastViewportWidth{ 0 }
		, m_lastViewportHeight{ 0 }
		, m_pCubeEntity{ nullptr }
	{}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGUIRender() override;
	virtual void OnEvent(kepler::Event& e) override;
};