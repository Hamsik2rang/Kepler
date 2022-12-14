#include "RayTracer.h"

RayTracer::RayTracer()
	: m_pSphere{nullptr}
{
	m_pSphere = new Sphere(kepler::Vec3f{ 0.0f, 0.0f, 100.0f }, 0.25f, kepler::Vec4f{0.0f, 0.0f, 1.0f, 1.0f});
}

RayTracer::~RayTracer()
{
	if (m_pSphere)
	{
		delete m_pSphere;
		m_pSphere = nullptr;
	}
}

kepler::Vec4f RayTracer::TraceRay(Ray& ray)
{
	const auto hitProp = m_pSphere->IntersectRayCollision(ray);
	if (hitProp.distance < 0.0f)
	{
		return kepler::Vec4f::Zero;
	}
	else
	{
		return m_pSphere->GetColor() * hitProp.distance;
	}
}
