#pragma once

#include "Camera.hpp"


namespace kepler {

	class PerspectiveCamera : public Camera
	{
	protected:
		Vec3f m_position;
		Vec3f m_at;
		float m_distance;
		float m_fovY;
		float m_aspect;
		float m_nearClip;
		float m_farClip;

		void CalcViewProjectionMatrix();

	public:
		PerspectiveCamera();
		PerspectiveCamera(const Vec3f& position, const Vec3f& focalPoint, const float distance, float fovY, float aspect, float nearClip, float farClip);
		~PerspectiveCamera() = default;

		inline float GetDistance() const { return m_distance; }
		inline void SetDistance(float distance) { m_distance = distance; m_at = (m_at - m_position).Normalize() * m_distance; CalcViewProjectionMatrix(); }
		inline Vec3f GetPosition() const { return m_position; }
		inline void SetPosition(const Vec3f& position) { m_position = position; CalcViewProjectionMatrix(); }
		inline Vec3f GetFocus() const { return m_at; }
		inline void SetFocus(const Vec3f& focalPoint) { m_at = focalPoint; }
		inline Vec3f GetDirection() const { return m_at - m_position; }
	};
}