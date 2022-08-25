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
		std::vector<std::vector<component::Base*>> m_pComponentTable;
		std::list<Entity*> m_pEntityList;
		std::string m_name;

	private:
		// \brief Entity�� �� Component�� �ͼӽ�ŵ�ϴ�.
		// \param entity �� Component�� �Ҵ���� Entity
		// \param pComponent �ͼӵ� Component�� �ּ�
		// \param index �ͼӵ� Component�� Component Table Index
		void Register(Entity* entity, component::Base* pComponent, component::eComponentIndex index);

		// \brief Entity�� ���� Ư�� ������Ʈ�� �����մϴ�.
		// \param entity Component�� ������ Entity
		// \param index ������ Component�� Component Table Index
		void Remove(Entity* entity, component::eComponentIndex index);

	public:
		Scene();
		Scene(const std::string& name);
		~Scene();

		// \brief ���ο� Entity�� �����մϴ�.
		// \return ������ �� Entity
		Entity* CreateEntity();

		// \brief ������ UUID�� ������ ���ο� Entity�� �����մϴ�.
		// \return ������ �� Entity
		Entity* CreateEntityWithUUID(UUID uuid);

		// \brief �����ϴ� Entity�� �ı��մϴ�.
		// �ı��� Entity�� ����� �� �����ϴ�.
		// \param entity �ı��� Entity
		void DestroyEntity(Entity* entity);

		// \brief Scene ��ü�� �ı��մϴ�.
		// ���Ե� ��� Entity�� Component�� �Բ� ���ŵ˴ϴ�.
		void Destroy();

		// \brief ���ø� �Ķ���ͷ� ���޹��� ������Ʈ���� ��� ���� ��� Entity�� �����մϴ�.
		// \tparam ...TComponent Entity�� ������ �־�� �ϴ� ������Ʈ��
		// \return ������ �����ϴ� Entity*�� std::vector �����̳�
		template <typename ... TComponent>
		std::vector<Entity*> GetAllEntitiesWith()
		{
			uint64_t flag = 0;
			std::vector<Entity*> view;
			flag = component::ConvertTypeToFlag<TComponent...>();
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