#pragma once

#include "Kepler.h"

enum class eColliderType
{
	None = 0,
	Box,
	Circle,
};

enum class eColliderCategory
{
	Player = 0,
	Enemy = 1,
	Ball = 2,
	Net = 3,
	Ground = 4,
	Wall = 5
};

struct CollisionData;

class GameObject
{
protected:
	eColliderType m_colliderType;
	eColliderCategory m_colliderCategory;
public:
	GameObject(eColliderType colliderType, eColliderCategory colliderCategory)
		: m_colliderType{ colliderType }
		, m_colliderCategory{ colliderCategory }
	{}

	eColliderType GetColliderType() const { return m_colliderType; }
	eColliderCategory GetColliderCategory() const { return m_colliderCategory; }

	virtual kepler::Vec2f GetPosition() const = 0;
	virtual kepler::Vec2f GetSize() const = 0;
	virtual kepler::Vec2f GetCurrentDirection() const = 0;
	virtual kepler::Vec2f GetLastDirection() const = 0;

	virtual void GetAdditionalColliderStatus(bool& pOutData) const {}
	virtual void OnCollision(CollisionData& data) {};
};