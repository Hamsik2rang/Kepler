#pragma once

#include "Camera.h"


namespace kepler {

	class OrthographicCamera : public Camera
	{
	private:
		Vec3f m_position;
		float m_rotation;
		float m_aspect;

		Mat44f m_viewMatrix;
		Mat44f m_viewProjectionMatrix;

		void CalculateViewMatrix();
	public:
		void SetProjection(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip);
		
		inline void SetPosition(const Vec3f& position) { m_position = position; CalculateViewMatrix(); }
		inline void SetRotation(const float rotation) { m_rotation = rotation; CalculateViewMatrix(); }

		inline const Vec3f& GetPosition() const { return m_position; }
		inline const float GetRotation() const { return m_rotation; }
		inline const Mat44f& GetViewMatrix() const { return m_viewMatrix; }
		inline const Mat44f& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
	};
}