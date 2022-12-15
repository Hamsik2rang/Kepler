#pragma once
#include "Hittable.h"


class Sphere : public IHittable
{
private:
	kepler::Vec3f m_center;
	float m_radius;
	kepler::Vec3f m_color;
	kepler::Vec3f m_ambiant;
	kepler::Vec3f m_diffuse;
	kepler::Vec3f m_specular;
	float m_ks;
	float m_alpha;

public:
	Sphere(kepler::Vec3f center, float radius, kepler::Vec3f color, kepler::Vec3f ambiant, kepler::Vec3f diffuse, kepler::Vec3f specular, float ks, float alpha)
		: m_center{ center }
		, m_radius{ radius }
		, m_color{ color }
		, m_ambiant{ ambiant }
		, m_diffuse{ diffuse }
		, m_specular{ specular }
		, m_ks{ ks }
		, m_alpha{ alpha }
	{}

	virtual HitProperty IntersectRayCollision(Ray& ray) const override;

	inline void SetCenter(kepler::Vec3f center) { m_center = center; }
	inline void SetRadius(float radius) { m_radius = radius; }
	inline void SetColor(kepler::Vec3f color) { m_color = color; }

	inline void SetAmbiant(kepler::Vec3f ambiant) { m_ambiant = ambiant; }
	inline void SetDiffuse(kepler::Vec3f diffuse) { m_diffuse = diffuse; }
	inline void SetSpecular(kepler::Vec3f specular) { m_specular = specular; }
	inline void SetKS(float ks) { m_ks = ks; }
	inline void SetAlpha(float alpha) { m_alpha = alpha; }

	inline kepler::Vec3f	GetCenter() const { return m_center; }
	inline float			GetRadius() const { return m_radius; }
	inline kepler::Vec3f	GetColor() const { return m_color; }

	inline kepler::Vec3f	GetAmbiant() const { return m_ambiant; }
	inline kepler::Vec3f	GetDiffuse() const { return m_diffuse; }
	inline kepler::Vec3f	GetSpecular() const { return m_specular; }
	inline float			GetKS() const { return m_ks; }
	inline float			GetAlpha() const { return m_alpha; }
};