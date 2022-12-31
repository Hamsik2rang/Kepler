#include "RayTracer.h"

#include <cmath>

RayTracer::RayTracer()
	: m_pSphere{ nullptr }
	, m_lightPos{ 0.0f, 2.0f, -1.0f }
{
	m_pSphere = new Sphere(
		kepler::Vec3f{ 0.0f, 0.0f, 3.0f },
		0.25f,
		kepler::Vec3f{ 0.1f, 0.1f, 0.1f },
		kepler::Vec3f{ 0.0f, 0.0f, 0.0f },
		kepler::Vec3f{ 0.0f, 0.0f, 1.0f },
		kepler::Vec3f{ 1.0f, 1.0f, 1.0f },
		0.8f,
		9.0f
	);
}

RayTracer::~RayTracer()
{
	if (m_pSphere)
	{
		delete m_pSphere;
		m_pSphere = nullptr;
	}
}

kepler::Vec3f RayTracer::TraceRay(Ray& ray)
{
	const auto hitProp = m_pSphere->IntersectRayCollision(ray);
	if (hitProp.distance < 0.0f)
	{
		return kepler::Vec3f::Zero;
	}
	else
	{
		const kepler::Vec3f lightVector = (m_lightPos - hitProp.point).Normalize();

		const float diffuse = std::max(kepler::Dot(hitProp.normal, lightVector), 0.0f);

		const kepler::Vec3f reflectVector = 2.0f * kepler::Dot(hitProp.normal, lightVector) * hitProp.normal - lightVector;
		const float specular = std::pow(std::max(0.0f, kepler::Dot(reflectVector, -ray.GetDirection().Normalize())), m_pSphere->GetAlpha());

		return m_pSphere->GetAmbiant() + m_pSphere->GetDiffuse() * diffuse + m_pSphere->GetSpecular() * specular * m_pSphere->GetKS();
	}
}
