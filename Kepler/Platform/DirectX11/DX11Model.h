#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace kepler {
	using namespace DirectX;

	class DX11Texture;

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	class DX11Model
	{
	public:
		DX11Model();
		DX11Model(const DX11Model& other);
		~DX11Model();

		// ���� �����͸� ���� �����Ͽ� �ҷ��� �� ����Ͽ� �ʱ�ȭ�ϼ���.
		bool Init(ID3D11Device* pDevice, VertexType* pVertices, const int vertex_count, const WCHAR* pTextureFilename);
		// ������ ���� ���� �ҷ��� �� ����Ͽ� �ʱ�ȭ�ϼ���.
		bool Init(ID3D11Device* pDevice, const char* pModelFilename, const WCHAR* pTextureFilename);
		void Shutdown();
		// �׸��⸦ �غ��ϱ� ���� �׷��� ������ ���ο� ���� ���ۿ� �ε��� ���۸� �����ϴ�.
		void Render(ID3D11DeviceContext* deviceContext);
	private:
		bool InitBuffers(ID3D11Device* pDivice);
		bool LoadVertices(const char* pFilename);
		bool LoadTexture(ID3D11Device* pDevice, const WCHAR* pFilename);
	private:
		// ���� ������
		VertexType* m_pVertices = nullptr;
		float* m_pFloatVertex = nullptr;
		// �ؽ�ó ������
		DX11Texture* m_pTexture = nullptr;
		// ���� ����
		ID3D11Buffer* m_pVertexBuffer = nullptr;
		int m_vertexCount = 0;
		// �ε��� ����
		ID3D11Buffer* m_pIndexBuffer = nullptr;
		int m_indexCount = 0;
	public:
		inline int GetIndexCount() { return m_indexCount; }
		ID3D11ShaderResourceView* GetTexture();
	};
}