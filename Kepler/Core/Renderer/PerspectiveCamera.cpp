#include "kepch.h"

#include "PerspectiveCamera.h"

namespace kepler {

	PerspectiveCamera::PerspectiveCamera()
		: m_position{ 0.0f, 0.0f, 0.0f }
		, m_at{ 0.0f, 0.0f, 0.0f }
		, m_distance{ 10.0f }
		, m_fovY{ 45.0f }
		, m_aspect{ 16.0f / 9.0f }
		, m_nearClip{ 0.1f }
		, m_farClip{ 1000.0f }
	{
		CalcViewProjectionMatrix();
	}

	PerspectiveCamera::PerspectiveCamera(const Vec3f& position, const Vec3f& focalPoint, const float distance, float fovY, float aspect, float nearClip, float farClip)
		: m_position{ position }
		, m_at{ focalPoint }
		, m_distance{ distance }
		, m_fovY{ fovY }
		, m_aspect{ aspect }
		, m_nearClip{ nearClip }
		, m_farClip{ farClip }
	{
		CalcViewProjectionMatrix();
	}

	void PerspectiveCamera::CalcViewProjectionMatrix()
	{
		m_viewMatrix = math::LookAt(m_position, m_at);
		m_projectionMatrix = math::Perspective(m_fovY, m_aspect, m_nearClip, m_farClip);
		m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
	}
}