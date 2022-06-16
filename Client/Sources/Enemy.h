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

	const float m_notMoveRange = 10.0f;

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
	bool MoveToTarget(const float targetX, float& outHorizontal);
};