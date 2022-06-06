#include "CollisionDetector.h"

std::list<std::shared_ptr<GameObject>> CollisionDetector::s_pColliderList;

void CollisionDetector::AddCollider(std::shared_ptr<GameObject> collider)
{
	s_pColliderList.push_back(collider);
}

void CollisionDetector::DeleteCollider(std::shared_ptr<GameObject> collider)
{
	s_pColliderList.remove(collider);
}

bool CollisionDetector::Detection(std::shared_ptr<GameObject>&outLeftCollider, std::shared_ptr<GameObject>&outRightCollider)
{



	return false;
}
