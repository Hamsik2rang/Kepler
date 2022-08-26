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

	}
}