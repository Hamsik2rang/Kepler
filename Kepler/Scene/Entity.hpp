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
		Scene* m_pScene;

	public:
		Entity(Scene* pScene);
		Entity(Scene* pScene, const UUID& uuid);
		~Entity();

		// \brief Entity�� UUID(Universally Unique ID)�� �����մϴ�.
		// \return Entity�� UUID
		inline const UUID GetID() const { return m_uuid; }

		// \brief Entity�� ���� ���� Component ������ ǥ���ϴ� �÷��׸� �����մϴ�.
		// \return ������ �ִ� Component�� ǥ���ϴ� 64��Ʈ �÷���
		inline const uint64_t GetComponentFlag() const { return m_componentFlag; }

		// \brief Entity�� ���ø� �Ķ���ͷ� ���޹��� Ÿ���� Component�� ������ �ִ��� ���θ� �����մϴ�. 
		// \tparam TComponent ���� ���θ� �Ǵ��� Component Ÿ��
		// \return �ش� Component�� ���� ����
		template <typename TComponent>
		bool HasComponent()
		{
			KEPLER_CORE_ASSERT(std::is_abstract_v<TComponent> == false, "Kepler not support abstract component type.");
			static_assert(std::is_base_of_v<IComponent, TComponent>);

			auto flag = TComponent::GetStaticFlag();
			return m_componentFlag & flag;
		}

		// \brief Entity�� ������ ������Ʈ�� �����մϴ�.
		// ���� ���Ÿ� ��û�� Component�� �����ϰ� ���� �ʴٸ� Assert ó���˴ϴ�.
		// \tparam TComponent �����Ϸ��� Component Ÿ��
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
			m_pScene->Remove(*this, index);

			auto flag = TComponent::GetStaticFlag();
			m_componentFlag &= ~flag;
		}

		// \brief Entity�� �� Component�� �߰��մϴ�.
		// ���� �߰��� ��û�� Component�� �̹� �����ϰ� �ִٸ� Assert ó���˴ϴ�.
		// \tparam TComponent Entity�� �߰��� Component Ÿ��
		// \tparam Args Component ���� �� �����ڿ� ������ ���� Ÿ�� ��(������ ���� �ʿ� ����)
		// \param args Component ���� �� �����ڿ� ������ ���ڵ�
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
			m_pScene->Register(this, pComponent, index);

			auto flag = TComponent::GetStaticFlag();
			m_componentFlag |= flag;
		}

		// \brief Entity�� ������ Component �� ���ø� �Ķ���ͷ� ����� Ÿ���� Component�� �����մϴ�.
		// \tparam TComponent ���޹��� Component Ÿ��
		// \return �ش� Component�� �ּҸ� �����մϴ�. ���� Entity�� ����� Ÿ���� Component�� ������ ���� ���� ���
		// nullptr�� �����մϴ�.
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