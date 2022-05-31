#pragma once

#include "Math/KeplerMath.h"

namespace kepler {

	class Camera
	{
	protected:
		Mat44f m_projectionMatrix;
		
	public:
		Camera() = default;
		virtual ~Camera() = default;

		inline const Mat44f& GetProjectionMatrix() const { return m_projectionMatrix; }
	};
}