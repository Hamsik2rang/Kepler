#pragma once

#include <vector>
#include <list>
#include <memory>
#include <unordered_map>

#include "Core/Base.h"
#include "Core/UUID.h"
#include "Component.hpp"

namespace kepler {

	class Entity;

	class Scene
	{
		friend class Entity;

	private:
		std::vector<std::vector<IComponent*>> m_pComponentTable;
		std::list<Entity*> m_pEntityList;
		std::string m_name;

	private:
		// \brief Entity에 새 Component를 귀속시킵니다.
		// \param pEntity 새 Component를 할당받은 Entity
		// \param pComponent 귀속된 Component의 주소
		// \param index 귀속된 Component의 Component Table Index
		void Register(Entity* pEntity, IComponent* pComponent, eComponentIndex index);

		// \brief Entity가 가진 특정 컴포넌트를 제거합니다.
		// \param pEntity Component를 소유한 Entity
		// \param index 제거할 Component의 Component Table Index
		void Remove(Entity* pEntity, eComponentIndex index);

		// \brief Entity를 리스트에서 제외합니다.
		// DestroyEntity와 달리 실제 메모리에서 파괴하지 않습니다.
		// \param pEntity 리스트에서 제외할 Entity
		void RemoveEntity(Entity* pEntity);

	public:
		Scene();
		Scene(const std::string& name);
		~Scene();

		// \brief 새로운 Entity를 생성합니다.
		// \return 생성된 빈 Entity
		Entity* CreateEntity();

		// \brief 지정된 UUID를 가지는 새로운 Entity를 생성합니다.
		// \return 생성된 빈 Entity
		Entity* CreateEntityWithUUID(UUID uuid);

		// \brief 존재하는 Entity를 파괴합니다.
		// 파괴된 Entity는 사용할 수 없습니다.
		// \param pEntity 파괴할 Entity
		void DestroyEntity(Entity* pEntity);

		// \brief Scene 전체를 파괴합니다.
		// 포함된 모든 Entity와 Component도 함께 제거됩니다.
		void Destroy();

		// \brief 탬플릿 파라미터로 전달받은 컴포넌트들을 모두 가진 모든 Entity를 리턴합니다.
		// \tparam ...TComponent Entity가 가지고 있어야 하는 컴포넌트들
		// \return 조건을 만족하는 Entity*의 std::vector 컨테이너
		template <typename ... TComponent>
		std::vector<Entity*> GetAllEntitiesWith()
		{
			uint64_t flag = 0;
			std::vector<Entity*> view;
			flag = ConvertTypeToFlag<TComponent...>();
			for (const auto entity : m_pEntityList)
			{
				if ((entity->GetComponentFlag() & flag) == flag)
				{
					view.emplace_back(entity);
				}
			}

			return view;
		}
	};
}