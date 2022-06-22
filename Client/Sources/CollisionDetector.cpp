#include "CollisionDetector.h"

std::vector<Collider2D*> CollisionDetector::s_pColliders;

void CollisionDetector::AddCollider(Collider2D* pCollider)
{
	s_pColliders.push_back(pCollider);
}

void CollisionDetector::DeleteCollider(Collider2D* pCollider)
{
	for (int i = 0; i < s_pColliders.size(); i++)
	{
		if (s_pColliders[i] == pCollider)
		{
			s_pColliders.erase(s_pColliders.begin() + i, s_pColliders.begin() + i + 1);
		}
	}
}

bool CollisionDetector::Detection()
{
	// TODO: Implement Broad phase

	for (int i = 0; i < s_pColliders.size() - 1; i++)
	{
		for (int j = i + 1; j < s_pColliders.size(); j++)
		{
			// Box to Box - AABBs
			if (s_pColliders[i]->GetType() == eColliderType::Box && s_pColliders[j]->GetType() == eColliderType::Box)
			{
				auto& leftObj = (s_pColliders[i]->GetPosition().x < s_pColliders[j]->GetPosition().x) ? s_pColliders[i] : s_pColliders[j];
				auto& rightObj = (s_pColliders[i]->GetPosition().x > s_pColliders[j]->GetPosition().x) ? s_pColliders[i] : s_pColliders[j];

				auto& lowerObj = (s_pColliders[i]->GetPosition().y < s_pColliders[j]->GetPosition().y) ? s_pColliders[i] : s_pColliders[j];
				auto& higherObj = (s_pColliders[i]->GetPosition().y > s_pColliders[j]->GetPosition().y) ? s_pColliders[i] : s_pColliders[j];

				// has collision occured
				if (leftObj->GetPosition().x + leftObj->GetSize().x / 2.0f > rightObj->GetPosition().x - rightObj->GetSize().x / 2.0f &&
					lowerObj->GetPosition().y + lowerObj->GetSize().y / 2.0f > higherObj->GetPosition().y - higherObj->GetSize().y / 2.0f)
				{
					//...
					CollisionData data{};
					data.pCollider = s_pColliders[i];
					data.pInfo = s_pColliders[i]->GetCollisionInfo();
					s_pColliders[j]->GetGameObject().OnCollision(data);

					data.pCollider = s_pColliders[j];
					data.pInfo = s_pColliders[j]->GetCollisionInfo();
					s_pColliders[i]->GetGameObject().OnCollision(data);
				}
			}
			// Circle to Circle - center-to-center distance
			else if (s_pColliders[i]->GetType() == eColliderType::Circle && s_pColliders[j]->GetType() == eColliderType::Circle)
			{
				if ((s_pColliders[i]->GetPosition() - s_pColliders[j]->GetPosition()).Length() < (s_pColliders[i]->GetSize().x / 2.0f) + (s_pColliders[j]->GetSize().x / 2.0f))
				{
					//...
					CollisionData data{};

					data.pCollider = s_pColliders[i];
					data.pInfo = s_pColliders[i]->GetCollisionInfo();
					s_pColliders[j]->GetGameObject().OnCollision(data);

					data.pCollider = s_pColliders[j];
					data.pInfo = s_pColliders[j]->GetCollisionInfo();
					s_pColliders[i]->GetGameObject().OnCollision(data);
				}
			}
			// Box(AABB) to Circle - segment-to-center distance
			else
			{
				bool isCollided{ false };

				auto& boxObj = s_pColliders[i]->GetType() == eColliderType::Box ? s_pColliders[i] : s_pColliders[j];
				auto& circleObj = s_pColliders[i]->GetType() == eColliderType::Circle ? s_pColliders[i] : s_pColliders[j];

				// left segment
				if (!isCollided)
				{
					kepler::Vec2f segPoint{};
					// 1. center of circle is lower than lowest point of left-segment
					if (boxObj->GetPosition().y - boxObj->GetSize().y / 2.0f > circleObj->GetPosition().y)
					{
						segPoint = { boxObj->GetPosition().x - boxObj->GetSize().x / 2.0f, boxObj->GetPosition().y - boxObj->GetSize().y / 2.0f };
					}
					// 2. center of circle is higher than highest point of left-segment
					else if (boxObj->GetPosition().y + boxObj->GetSize().y / 2.0f < circleObj->GetPosition().y)
					{
						segPoint = { boxObj->GetPosition().x - boxObj->GetSize().x / 2.0f, boxObj->GetPosition().y + boxObj->GetSize().y / 2.0f };
					}
					// 3. center of circle is in left-segment range
					else
					{
						segPoint = { boxObj->GetPosition().x - boxObj->GetSize().x / 2.0f, circleObj->GetPosition().y };
					}

					if ((segPoint - circleObj->GetPosition()).Length() < circleObj->GetSize().x / 2.0f)
					{
						isCollided = true;
					}
				}

				// right segment
				if (!isCollided)
				{
					kepler::Vec2f segPoint{};
					// 1. center of circle's y-pos is smaller than lowest point of right-segment's it
					if (boxObj->GetPosition().y - boxObj->GetSize().y / 2.0f > circleObj->GetPosition().y)
					{
						segPoint = { boxObj->GetPosition().x + boxObj->GetSize().x / 2.0f, boxObj->GetPosition().y - boxObj->GetSize().y / 2.0f };
					}
					// 2. center of circle's y-pos is bigger than highest point of right-segment's it
					else if (boxObj->GetPosition().y + boxObj->GetSize().y / 2.0f < circleObj->GetPosition().y)
					{
						segPoint = { boxObj->GetPosition().x + boxObj->GetSize().x / 2.0f, boxObj->GetPosition().y + boxObj->GetSize().y / 2.0f };
					}
					// 3. center of circle is in right-segment range
					else
					{
						segPoint = { boxObj->GetPosition().x + boxObj->GetSize().x / 2.0f, circleObj->GetPosition().y };
					}

					if ((segPoint - circleObj->GetPosition()).Length() < circleObj->GetSize().x / 2.0f)
					{
						isCollided = true;
					}
				}

				// top segment
				if (!isCollided)
				{
					kepler::Vec2f segPoint{};
					// 1. center of circle's x-pos is smaller than smallest point of top-segment's it
					if (boxObj->GetPosition().x - boxObj->GetSize().x / 2.0f > circleObj->GetPosition().x)
					{
						segPoint = { boxObj->GetPosition().x - boxObj->GetSize().x / 2.0f, boxObj->GetPosition().y + boxObj->GetSize().y / 2.0f };
					}
					// 2. center of circle's x-pos is bigger than biggest point of top-segment's it
					else if (boxObj->GetPosition().x + boxObj->GetSize().x / 2.0f < circleObj->GetPosition().x)
					{
						segPoint = { boxObj->GetPosition().x + boxObj->GetSize().x / 2.0f, boxObj->GetPosition().y + boxObj->GetSize().y / 2.0f };
					}
					else
					{
						segPoint = { circleObj->GetPosition().x, boxObj->GetPosition().y + boxObj->GetSize().y / 2.0f };
					}

					if ((segPoint - circleObj->GetPosition()).Length() < circleObj->GetSize().y / 2.0f)
					{
						isCollided = true;
					}
				}

				if (!isCollided)
				{
					// bottom segment
					kepler::Vec2f segPoint{};
					// 1. center of circle's x-pos is smaller than smallest point of bottom-segment's it
					if (boxObj->GetPosition().x - boxObj->GetSize().x / 2.0f > circleObj->GetPosition().x)
					{
						segPoint = { boxObj->GetPosition().x - boxObj->GetSize().x / 2.0f, boxObj->GetPosition().y - boxObj->GetSize().y / 2.0f };
					}
					// 2. center of circle's x-pos is bigger than biggest point of bottom-segment's it
					else if (boxObj->GetPosition().x + boxObj->GetSize().x / 2.0f < circleObj->GetPosition().x)
					{
						segPoint = { boxObj->GetPosition().x + boxObj->GetSize().x / 2.0f, boxObj->GetPosition().y - boxObj->GetSize().y / 2.0f };
					}
					else
					{
						segPoint = { circleObj->GetPosition().x, boxObj->GetPosition().y - boxObj->GetSize().y / 2.0f };
					}

					if ((segPoint - circleObj->GetPosition()).Length() < circleObj->GetSize().y / 2.0f)
					{
						isCollided = true;
					}
				}

				// 충돌이 일어난 경우(하나 이상의 AABB의 테두리가 원과 겹친 경우)
				if (isCollided)
				{
					CollisionData data{};

					data.pCollider = s_pColliders[i];
					data.pInfo = s_pColliders[i]->GetCollisionInfo();
					s_pColliders[j]->GetGameObject().OnCollision(data);

					data.pCollider = s_pColliders[j];
					data.pInfo = s_pColliders[j]->GetCollisionInfo();
					s_pColliders[i]->GetGameObject().OnCollision(data);
				}
			}
		}
	}

	return false;
}
