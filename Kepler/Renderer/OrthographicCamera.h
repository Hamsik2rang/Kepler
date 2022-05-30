#pragma once

#include "Camera.h"
#include "Core/Application.h"

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
		OrthographicCamera()
			: m_position{ Vec3f::Zero }
			, m_rotation{ 0.0f }
			, m_viewMatrix{Mat44f::Identity}
			, m_viewProjectionMatrix{ Mat44f::Identity }
			, m_aspect{ 16.0f / 9.0f }
		{
			IWindow* pWindow = Application::Get()->GetWindow();
			m_aspect = static_cast<float>(pWindow->GetWidth()) / static_cast<float>(pWindow->GetHeight());
		}
		~OrthographicCamera() = default;

		void SetProjection(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip);
		
		inline void SetPosition(const Vec3f& position) { m_position = position; CalculateViewMatrix(); }
		inline void SetRotation(const float rotation) { m_rotation = rotation; CalculateViewMatrix(); }

		inline const Vec3f& GetPosition() const { return m_position; }
		inline const float GetRotation() const { return m_rotation; }
		inline const float GetAspect() const { return m_aspect; }
		inline const Mat44f& GetViewMatrix() const { return m_viewMatrix; }
		inline const Mat44f& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
	};
}