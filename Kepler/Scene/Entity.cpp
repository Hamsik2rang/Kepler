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
		// Scene�� �����ϴ� ��� Scene�� �ı��� �ñ�ϴ�.
		if (m_pScene)
		{
			m_pScene->RemoveEntity(this);
		}
		// ���� Scene�� �������� �ʴ� ��� ���� ������Ʈ�� �����մϴ�.
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