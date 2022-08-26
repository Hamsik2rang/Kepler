#include "kepch.h"

#include "Entity.h"

namespace kepler {

	Entity::Entity(Scene* pScene)
		: Entity(pScene, UUID())
	{

	}

	Entity::Entity(Scene* pScene, const UUID& uuid)
		: m_pScene{ pScene }
		, m_uuid{ uuid }
		, m_componentFlag{ 0u }
	{

	}

	Entity::~Entity()
	{
		// Scene이 존재하는 경우 Scene에 파괴를 맡깁니다.
		if (m_pScene)
		{
			m_pScene->RemoveEntity(this);
		}
		// 만약 Scene이 존재하지 않는 경우 직접 컴포넌트를 제거합니다.
		else
		{
			for (auto component : m_pComponents)
			{
				if (component)
				{
					delete component;
					component = nullptr;
				}
			}
		}
	}
}