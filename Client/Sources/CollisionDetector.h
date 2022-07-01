#pragma once

#include <list>

#include "Kepler.h"
#include "GameObject.h"
#include "Collider2D.hpp"

struct CollisionData
{
	Collider2D* pCollider;
	void* pInfo;
};

// 충돌체들을 관리하며 충돌 여부 감시하는 객체
class CollisionDetector
{
private:
	static std::vector<Collider2D*>s_pColliders;

public:
	static void AddCollider(Collider2D*  pCollider);
	static void DeleteCollider(Collider2D* pCollider);
	static bool Detection();
};