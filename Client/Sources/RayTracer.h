#pragma once
#include "Kepler.h"

#include "Ray.h"
#include "Sphere.h"

class RayTracer
{
private:
	Sphere* m_pSphere;
	kepler::Vec3f m_lightPos;
public:
	RayTracer();
	~RayTracer();

	kepler::Vec3f TraceRay(Ray& ray);
	Sphere* GetSphere() { return m_pSphere; }
	kepler::Vec3f GetLightPos() const { return m_lightPos; }
	void SetLightPos(kepler::Vec3f position) { m_lightPos = position; }
};