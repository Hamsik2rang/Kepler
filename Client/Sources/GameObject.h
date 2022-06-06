#pragma once

#include "Kepler.h"

enum class eColliderType
{
	None = 0,
	Box,
	Circle,
};

class GameObject
{
protected:
	eColliderType m_colliderType;

public:
	GameObject(eColliderType colliderType)
		: m_colliderType{ colliderType }
	{}
	virtual kepler::Vec2f GetPosition() const = 0;
	virtual kepler::Vec2f GetDirection() const = 0;
};