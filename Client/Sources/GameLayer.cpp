#include "GameLayer.h"

void GameLayer::OnAttach()
{
	m_pPlayer = std::make_shared<Player>(kepler::Vec2f{ 100, -200 }, kepler::Vec2f{ 200.0f, 400.0f }, false, eColliderType::Box);
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
	m_pPlayer->OnRender();
}

void GameLayer::OnEvent(kepler::Event& e)
{

}