#include "kepch.h"

#include "Scene.h"
#include "Entity.h"

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

	void Scene::DestroyEntity(Entity* entity)
	{
		for (uint32_t i = 0; i < component::componentCount; i++)
		{
			auto flag = entity->GetComponentFlag();
			if (flag & BIT_UINT64(i))
			{
				Remove(entity, static_cast<component::eComponentIndex>(i));
			}
		}
		m_pEntityList.remove(entity);
		delete entity;
	}

	void Scene::Destroy()
	{
		for (auto entity : m_pEntityList)
		{
			DestroyEntity(entity);
		}
	}

	void Scene::Register(Entity* entity, component::Base* pComponent, component::eComponentIndex index)
	{
		m_pComponentTable[index].push_back(pComponent);
	}

	void Scene::Remove(Entity* entity, component::eComponentIndex index)
	{
		for (auto component : m_pComponentTable[index])
		{
			if (component->GetOwner() == entity)
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
