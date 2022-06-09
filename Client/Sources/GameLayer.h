#pragma once

#include "Kepler.h"
#include "Player.h"
#include "Ball.h"
#include "Level.h"
#include "Enemy.h"

class GameLayer : public kepler::Layer
{
private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Level> m_pLevel;
	std::shared_ptr<Ball> m_pBall;

	kepler::OrthographicCamera m_camera;

public:
	GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGUIRender() override;
	virtual void OnEvent(kepler::Event& e) override;
};