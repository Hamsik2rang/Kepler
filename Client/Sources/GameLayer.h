#pragma once

#include <imgui.h>

#include "Kepler.h"
#include "Player.h"
#include "Ball.h"
#include "Level.h"
#include "Enemy.h"

enum class eGameState
{
	Menu = 0,
	Ready,
	Play,
	Pause,
	GameOver
};

class GameLayer : public kepler::Layer
{
private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Level> m_pLevel;
	std::shared_ptr<Ball> m_pBall;

	kepler::OrthographicCamera m_camera;
	
	// GUI ���� ����
	float m_time;
	float m_readyTime;
	bool m_bBlink;
	ImFont* m_pFont;
	ImFont* m_pHollowFont;
	ImFont* m_pDebugFont;
	eGameState m_state;

	uint32_t m_playerScore;
	uint32_t m_enemyScore;

public:
	GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGUIRender() override;
	virtual void OnEvent(kepler::Event& e) override;
};