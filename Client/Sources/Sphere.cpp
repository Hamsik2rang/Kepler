#include "Sphere.h"

HitProperty Sphere::IntersectRayCollision(Ray& ray) const
{
	HitProperty hitProp={ -1.0f, kepler::Vec3f::Zero, kepler::Vec3f::Zero};
	const float b = 2.0f * kepler::Dot(ray.GetDirection(), ray.GetOrigin() - m_center);
	const float c = kepler::Dot(ray.GetOrigin() - m_center, ray.GetOrigin() - m_center) - m_radius * m_radius;
	const float nabla = b * b / 4.0f - c;

	if (nabla >= 0.0f)
	{
		const float d1 = -b / 2.0f + ::sqrt(nabla);
		const float d2 = -b / 2.0f - ::sqrt(nabla);
		hitProp.distance = d1 < d2 ? d1 : d2;

		hitProp.point = ray.GetOrigin() + ray.GetDirection() * hitProp.distance;
		hitProp.normal = (hitProp.point - m_center).Normalize();

	}


	return hitProp;
}
