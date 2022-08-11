#pragma once

#include <list>
#include "Core/Base.h"
#include "Entity.h"

namespace kepler {

	class Scene
	{
	private:
		std::list<Entity*> m_pEntities;
		std::string m_name;

	public:
		Scene()
			: m_name{ "Scene" } 
		{}

		Scene(const std::string& name)
			: m_name{ name }
		{}



		inline const std::string& GetName() { return m_name; }
	};
}