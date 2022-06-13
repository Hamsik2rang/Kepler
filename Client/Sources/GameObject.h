#pragma once

#include "Kepler.h"


enum class eColliderType;
enum class eColliderCategory;

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

	virtual ~GameObject() = default;

	eColliderType GetColliderType() const { return m_colliderType; }
	eColliderCategory GetColliderCategory() const { return m_colliderCategory; }

	virtual kepler::Vec2f GetPosition() const = 0;
	virtual kepler::Vec2f GetSize() const = 0;
	virtual kepler::Vec2f GetCurrentDirection() const = 0;
	virtual kepler::Vec2f GetLastDirection() const = 0;

	virtual void* GetCollisionInfo() const { return nullptr; }
	virtual void OnCollision(CollisionData& data) {};
};