#include "Render2DTestLayer.h"

void Render2DTestLayer::OnAttach()
{
	kepler::IWindow* window = kepler::Application::Get()->GetWindow();
	float height = static_cast<float>(window->GetHeight());
	float width= static_cast<float>(window->GetWidth());
	m_camera.SetProjection(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	m_camera.SetPosition({ 0.0f, 0.0f, 0.0f });
}

void Render2DTestLayer::OnDetach()
{


}

void Render2DTestLayer::OnUpdate(const float deltaTime)
{
	kepler::Renderer2D::Get()->BeginScene(m_camera);

	kepler::Renderer2D::Get()->DrawQuad({ 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	kepler::Renderer2D::Get()->DrawQuad({ 0.5f, 0.5f, 0.0f }, { 0.5, 0.5f }, { 0.0f, 0.8f, 1.0f, 1.0f });
	kepler::Renderer2D::Get()->DrawQuad({ -0.8f, -0.5f, 0.0f }, { 0.2f, 0.2f }, { 1.0f, 0.0f, 0.3f, 1.0f });

	
	kepler::Renderer2D::Get()->EndScene();
}

void Render2DTestLayer::OnEvent(kepler::Event & e)
{
	KEPLER_TRACE("{0}", e);
}
