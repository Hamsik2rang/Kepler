#pragma once

#include "Core/Base.h"
#include "Core/UUID.h"
#include "Scene.h"
#include "ECManager.hpp"

namespace kepler {

	class Entity
	{
	private:
		UUID m_uuid;
		Scene* m_pScene;

	public:
		Entity(Scene* pScene);
		Entity(Scene* pScene, const UUID& uuid);
		~Entity();

		const UUID GetID() const;

		template <typename TComponent>
		bool HasComponent()
		{
			return m_pScene->m_pManager->Has<TComponent>(m_uuid);
		}

		template <typename TComponent>
		void RemoveComponent()
		{
			KEPLER_CORE_ASSERT(HasComponent<TComponent>(), "Entity doesn't have the component of this type.");
			m_pScene->m_pManager->Remove<TComponent>(m_uuid);
		}

		template <typename TComponent, typename ... Args>
		void AddComponent(Args&& ... args)
		{
			KEPLER_CORE_ASSERT(!HasComponent<TComponent>(), "Entity already has the component of this type.");

			m_pScene->m_pManager->Emplace<TComponent, Args>(m_uuid, std::forward(args)...);
		}

		template <typename TComponent>
		TComponent* GetComponent()
		{
			if (HasComponent<TComponent>())
			{
				return m_pScene->m_pManager->Get<TComponent>(m_uuid);
			}

			return nullptr;
		}
	};
}