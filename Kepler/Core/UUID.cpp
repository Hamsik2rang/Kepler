#include "kepch.h"

#include "Core/UUID.h"
#include "Utility/Random.hpp"

namespace kepler {

	UUID::UUID()
		: m_uuid{ Random::Uint64() }
	{

	}

	UUID::UUID(uint64_t id)
		: m_uuid{ id }
	{

	}
}