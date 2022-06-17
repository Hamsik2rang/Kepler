#pragma once

#include "Kepler.h"
#include "Level.h"
#include "Player.h"
#include "Ball.h"

class Enemy : public Player
{
private:
	std::shared_ptr<Level> m_pLevel;
	std::shared_ptr<Ball> m_pBall;
	std::shared_ptr<Player> m_pPlayer;

	// 공이 움직일 예상 위치
	kepler::Vec2f m_ballNextPosition;
	// ChangeState() 인자
	int m_horizontal = 0;
	int m_vertical = 0;
	// 이동 목표 x 
	float m_targetX = 0.0f;
	// 이동 목표 x 달성 취급 범위
	float m_targetXRange = 5.0f;
	// 자신의 영역 범위
	float m_minX = 0.0f;
	float m_maxX = 0.0f;
	float m_minY = 0.0f;
	float m_maxY = 0.0f;
	kepler::Timer m_computeTimer;
public:
	Enemy(const kepler::Vec2f& position, const kepler::Vec2f& size, std::shared_ptr<Level> pLevel, std::shared_ptr<Player> pPlayer, std::shared_ptr<Ball> pBall, eColliderType type = eColliderType::Box, eColliderCategory category = eColliderCategory::Enemy);

	virtual void ChangeState(float deltaTime, int vertical, int horizontal) override;
	virtual void OnEvent(kepler::Event& e) override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void Respawn() override;
	// Inherited via GameObject
	virtual void OnCollision(CollisionData& data) override;

private:
	// 공의 예상 위치 계산
	void ComputeBallNextPosition(float deltaTime);
	bool MoveToTarget(int& outHorizontal);
	void Logic();
};