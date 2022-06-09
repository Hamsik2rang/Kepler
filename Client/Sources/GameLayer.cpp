#include "GameLayer.h"
#include "CollisionDetector.h"

#include <imgui.h>

GameLayer::GameLayer()
{}

void GameLayer::OnAttach()
{
	kepler::IWindow* window = kepler::Application::Get()->GetWindow();
	float height = static_cast<float>(window->GetHeight());
	float width = static_cast<float>(window->GetWidth());

	m_camera.SetProjection(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	m_camera.SetPosition({ 0.0f, 0.0f, 0.0f });

	m_pPlayer = std::make_shared<Player>(constant::PLAYER_SPAWN_POSITION, constant::SQUIRTLE_IDLE_SIZE, false);

	m_pBall = std::make_shared<Ball>(constant::BALL_PLAYER_SPAWN_POSITION, constant::BALL_SIZE.x);

	m_pLevel = std::make_shared<Level>();
	m_pLevel->Init(width, height);

	m_pEnemy = std::make_shared<Enemy>(kepler::Vec2f{ -constant::PLAYER_SPAWN_POSITION.x, constant::PLAYER_SPAWN_POSITION.y }, constant::SQUIRTLE_IDLE_SIZE, false);

	CollisionDetector::AddCollider(m_pPlayer);
	CollisionDetector::AddCollider(m_pBall);
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(float deltaTime)
{
	m_pLevel->OnUpdate(deltaTime);
	m_pBall->OnUpdate(deltaTime);
	m_pPlayer->OnUpdate(deltaTime);
	m_pEnemy->OnUpdate(deltaTime);
	CollisionDetector::Detection();
}

void GameLayer::OnRender()
{
	kepler::Renderer2D::Get()->BeginScene(m_camera);

	m_pLevel->OnRender();
	m_pBall->OnRender();
	m_pPlayer->OnRender();
	m_pEnemy->OnRender();

	kepler::Renderer2D::Get()->EndScene();
}

void GameLayer::OnGUIRender()
{
#ifdef _DEBUG
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Begin("Debug");

	// Player Info
	kepler::Vec2f playerPos = m_pPlayer->GetPosition();
	kepler::Vec2f playerSize = m_pPlayer->GetSize();
	kepler::Vec2f playerDir = m_pPlayer->GetCurrentDirection();
	kepler::Vec2f playerLastDir = m_pPlayer->GetLastDirection();
	ImGui::Text("Player");
	ImGui::Text("Position (%.2f, %.2f)", playerPos.x, playerPos.y);
	ImGui::Text("Size: (%.2f, %.2f)", playerSize.x, playerSize.y);
	ImGui::Text("Direction: (%.2f, %.2f)", playerDir.x, playerDir.y);
	ImGui::Text("Last Direction : (%.2f, %.2f)", playerLastDir.x, playerLastDir.y);

	ImGui::NewLine();
	
	kepler::Vec2f ballPos = m_pBall->GetPosition();
	kepler::Vec2f ballSize = m_pBall->GetSize();
	kepler::Vec2f ballDir = m_pBall->GetCurrentDirection();
	kepler::Vec2f ballLastDir = m_pBall->GetLastDirection();
	ImGui::Text("Ball");
	ImGui::Text("Position: (%.2f, %.2f)", ballPos.x, ballPos.y);
	ImGui::Text("Size: (%.2f, %.2f)", ballSize.x, ballSize.y);
	ImGui::Text("Direction: (%.2f, %.2f)", ballDir.x, ballDir.y);
	ImGui::Text("Last Direction : (%.2f, %.2f)", ballLastDir.x, ballLastDir.y);

	ImGui::NewLine();

	kepler::Vec2f enemyPos = m_pEnemy->GetPosition();
	kepler::Vec2f enemySize = m_pEnemy->GetSize();
	kepler::Vec2f enemyDir = m_pEnemy->GetCurrentDirection();
	kepler::Vec2f enemyLastDir = m_pEnemy->GetLastDirection();
	ImGui::Text("Enemy");
	ImGui::Text("Position (%.2f, %.2f)", enemyPos.x, enemyPos.y);
	ImGui::Text("Size: (%.2f, %.2f)", enemySize.x, enemySize.y);
	ImGui::Text("Direction: (%.2f, %.2f)", enemyDir.x, enemyDir.y);
	ImGui::Text("Last Direction : (%.2f, %.2f)", enemyLastDir.x, enemyLastDir.y);


	ImGui::End();
#endif
}

void GameLayer::OnEvent(kepler::Event& e)
{

}