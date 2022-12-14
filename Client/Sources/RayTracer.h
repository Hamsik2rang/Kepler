#pragma once
#include "Kepler.h"

#include "Ray.h"
#include "Sphere.h"

class RayTracer
{
private:
	Sphere* m_pSphere;
public:
	RayTracer();
	~RayTracer();

	kepler::Vec4f TraceRay(Ray& ray);

};