#include "GameLayer.h"

void GameLayer::OnAttach()
{
	m_pPlayer = new Player;
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(const float deltaTime)
{

}

void GameLayer::OnRender()
{
	m_pPlayer->OnRender();
}

void GameLayer::OnEvent(kepler::Event& e)
{
	// KeyPressed
	if (e.GetEventType() == kepler::eEventType::KeyPressed)
	{
		kepler::KeyPressedEvent* keyPressedEvent = reinterpret_cast<kepler::KeyPressedEvent*>(&e);
		switch (keyPressedEvent->GetKeyCode())
		{
		case kepler::key::Up:
			{

			}
			break;
		case kepler::key::Down:
			{

			}
			break;
		case kepler::key::Left:
			{

			}
			break;
		case kepler::key::Right:
			{

			}
			break;
		case kepler::key::Space:
			{

			}
			break;
		}
	}
}