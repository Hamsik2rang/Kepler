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

		// ī�޶��� ��ġ�� �����մϴ�.
		void SetPosition(float x, float y, float z);
		// ī�޶��� ȸ���� �����մϴ�.
		void SetRotation(float x, float y, float z);
		// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�.
		void Render();
	};
}