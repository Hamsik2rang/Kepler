#pragma once

#include <list>

#include "Kepler.h"
#include "GameObject.h"
#include "Collider2D.hpp"

struct CollisionData
{
	Collider2D* collider;
	kepler::Vec2f normal;
	void* bIsSpiked;
};

class CollisionDetector
{
private:
	static std::vector<Collider2D*>s_pColliders;

public:
	static void AddCollider(Collider2D*  collider);
	static void DeleteCollider(Collider2D* collider);
	static bool Detection();
};