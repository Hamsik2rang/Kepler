#include "kepch.h"

#include "Scene.h"
#include "Entity.h"
#include "ECManager.hpp"

namespace kepler {

	Scene::Scene()
		: m_name{ "Scene" }
	{

	}

	Scene::Scene(const std::string& name)
		: m_name{ name }
	{

	}

	Scene::~Scene()
	{
		Destroy();
	}

	Entity Scene::CreateEntity()
	{
		Entity entity{ this };
		m_pManager->Registry(entity.GetID());
		
		return entity;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		m_pManager->Destroy(entity.GetID());
	}

	void Scene::Destroy()
	{

	}
}
