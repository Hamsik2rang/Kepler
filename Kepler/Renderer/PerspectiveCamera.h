#pragma once

#include "Camera.hpp"


namespace kepler {

	class PerspectiveCamera : public Camera
	{
	protected:
		Vec3f m_position;
		Vec3f m_at;

		float m_pitch;
		float m_yaw;

		float m_distance;
		float m_fovY;
		float m_aspect;
		float m_nearClip;
		float m_farClip;

	public:
		PerspectiveCamera();
		PerspectiveCamera(const Vec3f& position, float distance, float fovY, float aspect, float nearClip, float farClip);
		~PerspectiveCamera() = default;

		inline float GetDistance() const { return m_distance; }
		inline void SetDistance(float distance) { m_distance = distance; }
		inline Vec3f GetPosition() const { return m_position; }
		inline void SetPosition(Vec3f& position) { m_position = position; }
		inline float GetPitch() const { return m_pitch; }
		inline float GetYaw() const { return m_yaw; }
	};
}