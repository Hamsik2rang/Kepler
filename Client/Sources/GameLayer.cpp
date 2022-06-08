#include "GameLayer.h"

#include <imgui.h>

void GameLayer::OnAttach()
{
	kepler::IWindow* window = kepler::Application::Get()->GetWindow();
	float height = static_cast<float>(window->GetHeight());
	float width = static_cast<float>(window->GetWidth());

	m_camera.SetProjection(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	m_camera.SetPosition({ 0.0f, 0.0f, 0.0f });


	kepler::Vec2f pos = { 280.0f, -230.0f};
	kepler::Vec2f size = { 152.5f, 200.0f };
	m_pPlayer = std::make_shared<Player>(pos, size, true, eColliderType::Box);

	m_pLevel = std::make_shared<Level>();
	m_pLevel->Init(width, height);

}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(float deltaTime)
{
	m_pLevel->OnUpdate(deltaTime);
	m_pPlayer->OnUpdate(deltaTime);
}

void GameLayer::OnRender()
{
	kepler::Renderer2D::Get()->BeginScene(m_camera);
	m_pLevel->OnRender();
	m_pPlayer->OnRender();
	kepler::Renderer2D::Get()->EndScene();
}

void GameLayer::OnGUIRender()
{
	kepler::Vec2f playerPos = m_pPlayer->GetPosition();
	kepler::Vec2f playerDir = m_pPlayer->GetCurrentDirection();
	kepler::Vec2f playerLastDir = m_pPlayer->GetLastDirection();
	ImGuiIO& io = ImGui::GetIO();
	
	ImGui::Begin("Debug");
	ImGui::Text("Player Position: (%.2f, %.2f)", playerPos.x, playerPos.y);
	ImGui::Text("Player Direction: (%.2f, %.2f)", playerDir.x, playerDir.y);
	ImGui::Text("Player Last Direction : (%.2f, %.2f)", playerLastDir.x, playerLastDir.y);
	
	
	
	ImGui::End();
}

void GameLayer::OnEvent(kepler::Event& e)
{

}