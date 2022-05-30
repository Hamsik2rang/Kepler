#include "Render2DTestLayer.h"

#include "Math/KeplerMath.h"

void Render2DTestLayer::OnAttach()
{
	kepler::IWindow* window = kepler::Application::Get()->GetWindow();
	float height = static_cast<float>(window->GetHeight());
	float width= static_cast<float>(window->GetWidth());
	m_camera.SetProjection(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	m_camera.SetPosition({ 0.0f, 0.0f, 0.0f });

	m_pTexture = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, "Assets/Textures/2k_earth_daymap.jpg");

}

void Render2DTestLayer::OnDetach()
{


}

void Render2DTestLayer::OnUpdate(const float deltaTime)
{
	kepler::Renderer2D::Get()->BeginScene(m_camera);

	float t = ::sinf(deltaTime);
	
	kepler::Renderer2D::Get()->DrawQuad({ -150.f * t, -300.0f * t, 0.0f }, 45.0f, { 100.0f, 100.0f }, { 0.0f, 0.8f, 1.0f, 1.0f });
	kepler::Renderer2D::Get()->DrawQuad({ t * 200, 0.0f, 0.0f }, { 150.f, 150.f }, m_pTexture, { 1.0f, 1.0f, 1.0f, 1.0f });
	kepler::Renderer2D::Get()->DrawQuad({ 300.0f, t * 200, 0.0f }, { 60.0f, 80.0f }, { 1.0f, 0.0f, 0.3f, 1.0f });
	kepler::Renderer2D::Get()->DrawQuad({ -300.0f, t * 200, 0.0f }, 360.0f * t, { 60.0f, 80.0f }, {sinf(deltaTime), cosf(deltaTime) * sinf(deltaTime /2.0f), 0.3f, 1.0f});
	
	kepler::Renderer2D::Get()->EndScene();
}

void Render2DTestLayer::OnEvent(kepler::Event & e)
{
	KEPLER_TRACE("{0}", e);
}
