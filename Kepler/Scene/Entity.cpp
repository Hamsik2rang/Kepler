#include "kepch.h"

#include "Entity.h"
#include "ECManager.hpp"

namespace kepler {

	Entity::Entity(Scene* pScene)
		: m_pScene{ pScene }
	{

	}

	Entity::Entity(Scene* pScene, const UUID& uuid)
		: m_pScene{ pScene }
		, m_uuid{ uuid }
	{

	}

	Entity::~Entity()
	{
		m_pScene->m_pManager->Destroy(m_uuid);
	}

	const UUID Entity::GetID() const
	{
		return m_uuid;
	}

}