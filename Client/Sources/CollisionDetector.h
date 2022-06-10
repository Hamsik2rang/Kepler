#pragma once

#include <list>

#include "Kepler.h"
#include "GameObject.h"

enum class eColliderType
{
	None = 0,
	Box,
	Circle,
};

enum class eColliderCategory
{
	Player = 0,
	Enemy,
	Ball,
	Net,
	Ground,
	Wall, 
	Sky,
};

struct CollisionData
{
	std::shared_ptr<GameObject> collider;
	kepler::Vec2f normal;
	bool bIsSpiked;
};

class CollisionDetector
{
private:
	static std::vector<std::shared_ptr<GameObject>> s_pColliders;

public:
	static void AddCollider(std::shared_ptr<GameObject> collider);
	static void DeleteCollider(std::shared_ptr<GameObject> collider);
	static bool Detection();
};