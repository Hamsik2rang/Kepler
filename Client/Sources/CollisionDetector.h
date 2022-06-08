#pragma once

#include <list>

#include "Kepler.h"
#include "GameObject.h"


struct CollisionData
{
	kepler::Vec2f collisionPos;
	std::shared_ptr<GameObject> collider;
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