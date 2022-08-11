#pragma once

#include <functional>

#include "Core/Base.h"
#include "ComponentType.h"

namespace kepler {

	struct IComponent;
	class Entity
	{
	private:
		std::string m_name;
		std::function<void()> m_pDestroy;
		std::list<IComponent*> m_pComponents;

	public:
		Entity();
		Entity(const std::string& name);
		Entity(const Entity& entity) = default;
		~Entity();

		template <typename TComp>
		void AddComponent()
		{
			if (HasComponent<TComp>())
			{
				// assert
			}


		}
		
		template <typename TComp>
		TComp* GetComponent()
		{
			if (HasComponent<TComp>())
			{
				// assert
			}


		}

		template <typename TComp>
		void RemoveComponent()
		{
			if (HasComponent<TComp>())
			{
				// assert
			}


		}

		template <typename TComp>
		const bool HasComponent()
		{

		}

		void OnUpdate(const float deltaTime);
		inline const void OnDestroy() { m_pDestroy(); }

		inline const std::string& GetName() { return m_name; }
	};
}