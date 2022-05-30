#include "kepch.h"

#include "OrthographicCamera.h"

namespace kepler {

	void OrthographicCamera::CalculateViewMatrix()
	{
		//m_viewMatrix = math::GetTransformMatrix(m_position) * math::GetRotationMatrixZ(math::DegToRad(m_rotation));
		m_viewMatrix = Mat44f::Identity;
		m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
	}

	void OrthographicCamera::SetProjection(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip)
	{
		m_aspect = (right - left) / (bottom - top);
		m_projectionMatrix = math::Orthographic(left, right, top, bottom, nearClip, farClip);
	}

}