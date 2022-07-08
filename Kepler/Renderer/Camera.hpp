#pragma once

#include "KeplerMath.h"

namespace kepler {

	class Camera
	{
	protected:
		Mat44f m_viewMatrix;
		Mat44f m_projectionMatrix;
		
	public:
		Camera() 
			: m_viewMatrix{ Mat44f::Identity }
			, m_projectionMatrix{ Mat44f::Identity }
		{}
		virtual ~Camera() = default;

		inline const Mat44f& GetViewMatrix() const { return m_viewMatrix; }
		inline const Mat44f& GetProjectionMatrix() const { return m_projectionMatrix; }
		inline const Mat44f& GetViewProjectionMatrix() const { return m_viewMatrix * m_projectionMatrix; }
	};
}