#include "kepch.h"

#include "Entity.h"
#include "Component.hpp"

namespace kepler {

	Entity::Entity()
		: m_name{ "Entity" }
		, m_pDestroy{ nullptr }
		, m_pComponents{}
	{}

	Entity::Entity(const std::string& name)
		: m_name{ name }
		, m_pDestroy{ nullptr }
		, m_pComponents{}
	{}

	Entity::~Entity()
	{
		//...
	}



}