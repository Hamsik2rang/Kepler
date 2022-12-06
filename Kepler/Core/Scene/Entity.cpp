#include "kepch.h"

#include "Entity.hpp"

namespace kepler {

Entity::Entity(Scene* pScene)
	: Entity(pScene, UUID())
{

}

	Entity::Entity(Scene* pScene, const UUID& uuid)
		: m_scene{ pScene }
		, m_uuid{ uuid }
		, m_componentFlag{ 0u }
		, m_pParent{ nullptr }
	{

}

Entity::~Entity()
{

	}

	void Entity::SetParentEntity(Entity* pEntity)
	{
		m_pParent = pEntity;
	}

	void Entity::AddChildEntity(Entity* pEntity)
	{
		if (std::find(m_pChildren.begin(), m_pChildren.end(), pEntity) != m_pChildren.end())
		{
			return;
		}
		m_pChildren.push_back(pEntity);
	}
}