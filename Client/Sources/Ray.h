#pragma once
#include "Kepler.h"

class Ray
{
private:
	kepler::Vec3f m_origin;
	kepler::Vec3f m_direction;

public:
	Ray(kepler::Vec3f origin, kepler::Vec3f direction)
		: m_origin{ origin }
		, m_direction{ direction }
	{}

	kepler::Vec3f GetDirection() const { return m_direction; }
	kepler::Vec3f GetOrigin() const { return m_origin; }
};