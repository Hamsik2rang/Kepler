#pragma once

#include "Core/Base.h"
#include "Component.hpp"
#include "Core/UUID.h"

#include <algorithm>

namespace kepler {

	class Entity;

	class ECManager
	{
	private:
		// (Im Yongsik)TODO: 추후 정의된 컴포넌트 수를 알 수 있는 방법(ex. reflection)이 구현되면 대체할 것.
		// 그 전까진 현재 정의된 컴포넌트 타입 개수 이상만큼 직접 수정해 줘야 함.
		static uint8_t s_componentTypeCount;

		std::unordered_map<uint64_t, std::vector<component::Base*>> m_pComponentTable;
		std::list<UUID> m_entityUUIDList;

	private:
		template <typename TComponent, typename ... TOther>
		bool Has(UUID uuid)
		{
			return (Has<TComponent>(uuid) && Has<TOther>(uuid));
		}

	public:
		ECManager();

		void Registry(UUID uuid)
		{
			if (std::find(m_entityUUIDList.begin(), m_entityUUIDList.end(), uuid) != m_entityUUIDList.end())
			{
				return;
			}
			std::vector<component::Base*> v(s_componentTypeCount);
			m_entityUUIDList.push_back(uuid);
			m_pComponentTable[(uint64_t)uuid] = v;
		}

		void Destroy(UUID uuid)
		{
			auto iter = std::find(m_entityUUIDList.begin(), m_entityUUIDList.end(), uuid);
			if (iter == m_entityUUIDList.end())
			{
				// assert
			}

			for (auto component : m_pComponentTable[(uint64_t)uuid])
			{
				if (component)
				{
					delete component;
					component = nullptr;
				}
			}
		}

		template <typename TComponent, typename ... Args>
		void Emplace(UUID uuid, Args&& ... args)
		{
			KEPLER_CORE_ASSERT(!Has<TComponent>(uuid), "This Entity already has the component of thie type");

			uint32_t index = TComponent::GetStaticIndex();
			m_pComponentTable[(uint64_t)uuid][index] = new TComponent(std::forward<Args>(args)...);
		}

		template <typename TComponent>
		void Remove(UUID uuid)
		{
			KEPLER_CORE_ASSERT(Has<TComponent>(uuid), "This Entity doesn't have the component of this type");

			uint32_t index = TComponent::GetStaticIndex();
			delete m_pComponentTable[(uint64_t)uuid][index];
			m_pComponentTable[(uint64_t)uuid][index] = nullptr;
		}

		template <typename TComponent>
		void Patch(UUID uuid, TComponent* pComponent)
		{
			uint32_t index = TComponent::GetStaticIndex();
			if (m_pComponentTable[(uint64_t)uuid][index])
			{
				Remove<TComponent>(uuid);
			}
			m_pComponentTable[(uint64_t)uuid][index] = pComponent;
		}

		template <typename TComponent>
		TComponent* Get(kepler::UUID uuid)
		{
			uint32_t index = TComponent::GetStaticType();
			if (m_pComponentTable[(uint64_t)uuid][index])
			{
				return m_pComponentTable[(uint64_t)uuid][index];
			}
		
			return nullptr;
		}

		// for test - not work.
		//template <typename TComponent>
		//TComponent* Get(const Entity& entity)
		//{
		//	uint32_t index = TComponent::GetStaticType();
		//	if (m_pComponentTable[(uint64_t)entity.GetID()][index])
		//	{
		//		return m_pComponentTable[(uint64_t)entity.GetID()][index];
		//	}

		//	return nullptr;
		//}

		//template <typename TComponent, typename ... TOther>
		//std::shared_ptr<Entity[]> View()
		//{
		//	std::vector<Entity> temp;
		//	for (const auto& pEntity : m_entityList)
		//	{
		//		if (Has<TComponent>(pEntity) && Has<TOther>(pEntity))
		//		{
		//			temp.push_back(pEntity);
		//		}
		//	}
		//	auto result = std::make_shared<Entity[]>(temp.size());
		//	for (int i = 0; i < temp.size(); i++)
		//	{
		//		result[i] = temp[i];
		//	}
		// 
		//	return result;
		//}

		template <typename TComponent>
		bool Has(UUID uuid)
		{
			if (m_pComponentTable[(uint64_t)uuid][TComponent::GetStaticIndex()])
			{
				return true;
			}

			return false;
		}
		
	};
}