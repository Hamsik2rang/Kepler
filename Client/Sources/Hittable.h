#pragma once
#include "Kepler.h"
#include "Ray.h"
struct HitProperty
{
	float distance;
	kepler::Vec3f point;
	kepler::Vec3f normal;
};

class IHittable
{
public:
	virtual ~IHittable() = default;

	virtual HitProperty IntersectRayCollision(Ray& ray) const = 0;
};