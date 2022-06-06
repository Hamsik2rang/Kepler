#pragma once

#include <list>

#include "Kepler.h"
#include "GameObject.h"


class CollisionDetector
{
private:
	static std::list<std::shared_ptr<GameObject>> s_pColliderList;

public:
	static void AddCollider(std::shared_ptr<GameObject> collider);
	static void DeleteCollider(std::shared_ptr<GameObject> collider);
	static bool Detection(std::shared_ptr<GameObject>& outLeftCollider, std::shared_ptr<GameObject>& outRightCollider);
};