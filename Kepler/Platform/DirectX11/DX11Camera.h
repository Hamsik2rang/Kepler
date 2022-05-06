#pragma once

#include <DirectXMath.h>

namespace kepler {

	using namespace DirectX;

	class DX11Camera
	{
	private:
		XMFLOAT3 m_position{};
		XMFLOAT3 m_rotation{};
		XMMATRIX m_viewMatrix{};

	public:
		inline XMFLOAT3 GetPosition() const { return m_position; };
		inline XMFLOAT3 GetRotation() const { return m_rotation; };
		inline void GetViewMatrix(XMMATRIX& viewMatrix) { viewMatrix = m_viewMatrix; };

		DX11Camera();
		DX11Camera(const DX11Camera& other);
		~DX11Camera();

		// 카메라의 위치를 설정합니다.
		void SetPosition(float x, float y, float z);
		// 카메라의 회전을 설정합니다.
		void SetRotation(float x, float y, float z);
		// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
		void Render();
	};
}