#pragma once

#include <stdint.h>
#include <type_traits>

namespace kepler {

	class UUID
	{
	private:
		uint64_t m_uuid;

	public:
		UUID();
		UUID(uint64_t id);
		UUID(const UUID& uuid) = default;

		inline operator uint64_t() const { return m_uuid; }
	};
}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<kepler::UUID>
	{
		std::size_t operator()(const kepler::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};
}