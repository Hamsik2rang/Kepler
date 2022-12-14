#pragma once
#include "Hittable.h"


class Sphere : public IHittable
{
private:
	kepler::Vec3f m_center;
	float m_radius;
	kepler::Vec4f m_color;

public:
	Sphere(kepler::Vec3f center, float radius, kepler::Vec4f color)
		: m_center{ center }
		, m_radius{ radius }
		, m_color{ color }
	{}

	virtual HitProperty IntersectRayCollision(Ray& ray) const override;

	inline kepler::Vec3f GetCenter() const { return m_center; }
	inline float GetRadius() const { return m_radius; }
	inline kepler::Vec4f GetColor() const { return m_color; }
	inline void SetCenter(kepler::Vec3f center) { m_center = center; }
	inline void SetRadius(float radius) { m_radius = radius; }
	inline void SetColor(kepler::Vec4f color) { m_color = color; }
};