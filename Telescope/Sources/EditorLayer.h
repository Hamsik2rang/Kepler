#pragma once


#include <Kepler.h>

class EditorLayer : public kepler::Layer
{
private:
	kepler::EditorCamera m_camera;
	std::shared_ptr<kepler::IVertexArray> m_pCubeVA;
	float m_time;
public:

	EditorLayer()
		: Layer("Editor")
		, m_camera{ kepler::EditorCamera{ kepler::Vec3f{ 0.0f, 1.0f, -6.0f }, kepler::Vec3f{ 0.0f, 1.0f, 0.0f }, 10.0f, 45.0f, 16.0f / 9.0f, 0.1f, 1000.0f } }
		, m_pCubeVA{ nullptr }
		, m_time{ 0.0f }
	{}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGUIRender() override;
	virtual void OnEvent(kepler::Event& e) override;
};