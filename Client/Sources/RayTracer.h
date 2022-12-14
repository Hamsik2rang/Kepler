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
	inline void SetCenter(kepler::Vec3f center) { m_pSphere->SetCenter(center); }
	inline void SetRadius(float radius) { m_pSphere->SetRadius(radius); }
	inline void SetColor(kepler::Vec4f color) { m_pSphere->SetColor(color); }
};