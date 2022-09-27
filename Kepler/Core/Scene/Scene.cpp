#include "kepch.h"

#include "Scene.h"
#include "Entity.hpp"

namespace kepler {

	Scene::Scene()
		: Scene("Scene")
	{

	}

	Scene::Scene(const std::string& name)
		: m_name{ name }
		, m_pComponentTable{ component::componentCount }
	{

	}

	Scene::~Scene()
	{
		Destroy();
	}

	Entity* Scene::CreateEntity()
	{
		return CreateEntityWithUUID(UUID());
	}

	Entity* Scene::CreateEntityWithUUID(UUID uuid)
	{
		Entity* pEntity = new Entity(this, uuid);
		m_pEntityList.push_back(pEntity);

		return pEntity;
	}

	void Scene::DestroyEntity(Entity* pEntity)
	{
		for (uint32_t i = 0; i < component::componentCount; i++)
		{
			auto flag = pEntity->GetComponentFlag();
			if (flag & BIT_UINT64(i))
			{
				Remove(pEntity, static_cast<ComponentIndex>(i));
			}
		}
		m_pEntityList.remove(pEntity);

		delete pEntity;
	}

	void Scene::Destroy()
	{
		for (int i = 0; i < m_pEntityList.size(); i++)
		{
			DestroyEntity(m_pEntityList.front());
		}
	}

	void Scene::Register(Entity* pEntity, IComponent* pComponent, ComponentIndex index)
	{
		m_pComponentTable[index].push_back(pComponent);
	}

	void Scene::Remove(Entity* pEntity, ComponentIndex index)
	{
		for (auto component : m_pComponentTable[index])
		{
			if (component->GetOwner() == pEntity)
			{
				delete component;
				component = nullptr;

				std::swap(component, m_pComponentTable[index].back());
				m_pComponentTable[index].pop_back();

				break;
			}
		}
	}
}
