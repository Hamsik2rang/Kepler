#include "kepch.h"

#include "PerspectiveCamera.h"

namespace kepler {

	PerspectiveCamera::PerspectiveCamera()
		: m_aspect{ 16.0f / 9.0f }
		, m_fovY{ 45.0f }
		, m_nearClip{ 0.1f }
		, m_farClip{ 1000.0f }
		, m_pitch{ 0.0f }
		, m_yaw{ 0.0f }
		, m_position{ 0.0f,0.0f, 0.0f }
		, m_at{ 0.0f,0.0f, 0.0f }
		, m_distance{ 10.0f }
	{

	}

	PerspectiveCamera::PerspectiveCamera(const Vec3f& position, float distance, float fovY, float aspect, float nearClip, float farClip)
		: m_aspect{ aspect }
		, m_fovY{ fovY }
		, m_nearClip{ nearClip }
		, m_farClip{ farClip }
		, m_pitch{ 0.0f }
		, m_yaw{ 0.0f }
		, m_position{ position }
		, m_at{ 0.0f, 0.0f, 0.0f }
		, m_distance{ 10.0f }
	{

	}


}