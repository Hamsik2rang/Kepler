#pragma once

#include "Kepler.h"
#include "Player.h"


class Enemy : public Player
{
private:


public:
	Enemy(const kepler::Vec2f& position, const kepler::Vec2f& size, bool bIsJumped, eColliderType type = eColliderType::Box, eColliderCategory category = eColliderCategory::Enemy);
	virtual void OnEvent(kepler::Event& e) override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	// Inherited via GameObject
	virtual void OnCollision(CollisionData& data) override;
};