#pragma once

#include <utility>
#include <type_traits>

#include "Core/Base.h"
#include "Core/UUID.h"
#include "Component.hpp"
#include "Scene.h"

namespace kepler {

	class Scene;

	class Entity
	{
	private:
		UUID m_uuid;
		std::vector<IComponent*> m_pComponents;
		uint64_t m_componentFlag;
		Scene* m_scene;

	public:
		Entity(Scene* pScene);
		Entity(Scene* pScene, const UUID& uuid);
		~Entity();

		// \brief Entity의 UUID(Universally Unique ID)를 리턴합니다.
		// \return Entity의 UUID
		inline const UUID GetID() const { return m_uuid; }

		// \brief Entity가 현재 가진 Component 종류를 표현하는 플래그를 리턴합니다.
		// \return 가지고 있는 Component를 표현하는 64비트 플래그
		inline const uint64_t GetComponentFlag() const { return m_componentFlag; }

		// \brief Entity가 탬플릿 파라미터로 전달받은 타입의 Component를 가지고 있는지 여부를 리턴합니다. 
		// \tparam TComponent 소유 여부를 판단할 Component 타입
		// \return 해당 Component의 소유 여부
		template <typename TComponent>
		bool HasComponent()
		{
			KEPLER_CORE_ASSERT(std::is_abstract_v<TComponent> == false, "Kepler not support abstract component type.");
			static_assert(std::is_base_of_v<IComponent, TComponent>);

			auto flag = TComponent::GetStaticFlag();
			return m_componentFlag & flag;
		}

		// \brief Entity가 소유한 컴포넌트를 제거합니다.
		// 만약 제거를 요청한 Component를 소유하고 있지 않다면 Assert 처리됩니다.
		// \tparam TComponent 제거하려는 Component 타입
		//
		template <typename TComponent>
		void RemoveComponent()
		{
			if (!HasComponent<TComponent>())
			{
				KEPLER_CORE_ASSERT(false, "Entity doesn't have the component of this type.");
				return;
			}

			auto index = TComponent::GetStaticIndex();
			m_scene->Remove(*this, index);

			auto flag = TComponent::GetStaticFlag();
			m_componentFlag &= ~flag;
		}

		// \brief Entity에 새 Component를 추가합니다.
		// 만약 추가를 요청한 Component를 이미 소유하고 있다면 Assert 처리됩니다.
		// \tparam TComponent Entity에 추가할 Component 타입
		// \tparam Args Component 생성 시 생성자에 전달할 인자 타입 팩(별도의 전달 필요 없음)
		// \param args Component 생성 시 생성자에 전달할 인자들
		//
		template <typename TComponent, typename ... Args>
		void AddComponent(Args&& ... args)
		{
			if (HasComponent<TComponent>())
			{
				KEPLER_CORE_ASSERT(false, "Entity already has the component of this type.");
				return;
			}

			TComponent* pComponent = new TComponent(this, std::forward<Args>(args)...);
			auto index = TComponent::GetStaticIndex();
			m_pComponents.push_back(pComponent);
			pComponent->SetOwner(this);
			m_scene->Register(this, pComponent, index);

			auto flag = TComponent::GetStaticFlag();
			m_componentFlag |= flag;
		}

		// \brief Entity가 소유한 Component 중 탬플릿 파라미터로 명시한 타입의 Component를 리턴합니다.
		// \tparam TComponent 전달받을 Component 타입
		// \return 해당 Component의 주소를 리턴합니다. 만약 Entity가 명시한 타입의 Component를 가지고 있지 않을 경우
		// nullptr을 리턴합니다.
		template <typename TComponent>
		TComponent* GetComponent()
		{
			KEPLER_CORE_ASSERT(std::is_abstract_v<TComponent> == false, "Kepler not support abstract component type.");
			static_assert(std::is_base_of_v<IComponent, TComponent>);

			auto index = TComponent::GetStaticIndex();
			if (HasComponent<TComponent>())
			{
				for (const auto component : m_pComponents)
				{
					if (component->GetFlag() & TComponent::GetStaticFlag())
					{
						return dynamic_cast<TComponent*>(component);
					}
				}
			}

			return nullptr;
		}
	};
}