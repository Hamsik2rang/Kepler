#include "GameLayer.h"

void GameLayer::OnAttach()
{
	kepler::IWindow* window = kepler::Application::Get()->GetWindow();
	float height = static_cast<float>(window->GetHeight());
	float width = static_cast<float>(window->GetWidth());

	m_camera.SetProjection(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	m_camera.SetPosition({ 0.0f, 0.0f, 0.0f });


	kepler::Vec2f pos = { 0.0f, 0.0f};
	kepler::Vec2f size = { 50.0f, 100.0f };
	m_pPlayer = std::make_shared<Player>(pos, size, false, eColliderType::Box);


}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(float deltaTime)
{
	m_pPlayer->OnUpdate(deltaTime);
}

void GameLayer::OnRender()
{
	kepler::Renderer2D::Get()->BeginScene(m_camera);

	m_pPlayer->OnRender();
	
	kepler::Renderer2D::Get()->EndScene();
}

void GameLayer::OnEvent(kepler::Event& e)
{

}