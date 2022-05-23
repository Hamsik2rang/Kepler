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

		// 정점 데이터를 직접 생성하여 불러올 때 사용하여 초기화하세요.
		bool Init(ID3D11Device* pDevice, VertexType* pVertices, const int vertex_count, const WCHAR* pTextureFilename);
		// 파일을 통해 모델을 불러올 때 사용하여 초기화하세요.
		bool Init(ID3D11Device* pDevice, const char* pModelFilename, const WCHAR* pTextureFilename);
		void Shutdown();
		// 그리기를 준비하기 위해 그래픽 파이프 라인에 정점 버퍼와 인덱스 버퍼를 놓습니다.
		void Render(ID3D11DeviceContext* deviceContext);
	private:
		bool InitBuffers(ID3D11Device* pDivice);
		bool LoadVertices(const char* pFilename);
		bool LoadTexture(ID3D11Device* pDevice, const WCHAR* pFilename);
	private:
		// 정점 데이터
		VertexType* m_pVertices = nullptr;
		float* m_pFloatVertex = nullptr;
		// 텍스처 데이터
		DX11Texture* m_pTexture = nullptr;
		// 정점 버퍼
		ID3D11Buffer* m_pVertexBuffer = nullptr;
		int m_vertexCount = 0;
		// 인덱스 버퍼
		ID3D11Buffer* m_pIndexBuffer = nullptr;
		int m_indexCount = 0;
	public:
		inline int GetIndexCount() { return m_indexCount; }
		ID3D11ShaderResourceView* GetTexture();
	};
}