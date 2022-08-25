#pragma once

#include <vector>
#include <list>
#include <unordered_map>

#include "Core/Base.h"
#include "Component.hpp"
#include "ECManager.hpp"

namespace kepler {

	class Entity;
	class ECManager;

	class Scene
	{
		friend class Entity;

	private:
		std::unique_ptr<ECManager> m_pManager;
		std::string m_name;

	private:
		template <typename TComponent, typename ... Args>
		void Emplace(const Entity& entity, Args&& ... args)
		{
			m_pManager->Emplace<TComponent, Args>(entity.GetID(), args...);
		}
		
		template <typename TComponent>
		void Remove(const Entity& entity)
		{
			m_pManager->Remove<TComponent>(entity.GetID());
		}
		
		template <typename TComponent>
		void Patch(const Entity& entity, TComponent* pComponent)
		{
			m_pManager->Patch(entity.GetID(), pComponent);
		}
		
		// NOTE: 추후 구현될 때까지 사용할 수 없습니다.
		template <typename TComponent, typename ... TOther>
		[[deprecated]]
		std::shared_ptr<Entity[]> GetAllEntitiesWith()
		{
			//return m_pManager->View();
			return nullptr;
		}
		
		template <typename TComponent>
		TComponent* Get(const Entity& entity)
		{
			return m_pManager->Get<TComponent>(entity.GetID());
		}
		
		template <typename TComponent>
		bool Has(const Entity& entity)
		{
			return m_pManager->Has<TComponent>(entity.GetID());
		}

	public:
		Scene();
		Scene(const std::string& name);
		~Scene();

		Entity CreateEntity();
		void DestroyEntity(const Entity& entity);
		void Destroy();
	};
}