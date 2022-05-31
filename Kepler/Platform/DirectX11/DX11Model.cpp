#include "kepch.h"
#include "DX11Model.h"
#include "DX11Texture.h"
#include "Renderer/Shader.h"

#include <fstream>
using namespace std;

namespace kepler {
	DX11Model::DX11Model()
	{
	}

	DX11Model::DX11Model(const DX11Model& other)
	{
	}

	DX11Model::~DX11Model()
	{

	}

	bool DX11Model::Init(ID3D11Device* pDevice, VertexType* pVertices, const int vertex_count, const WCHAR* pTextureFilename)
	{
		// 정점 데이터를 받습니다.
		m_pVertices = pVertices;
		m_vertexCount = vertex_count;
		m_indexCount = m_vertexCount;

		// 정점 및 인덱스 버퍼를 초기화합니다.
		if (!InitBuffers(pDevice))
		{
			return false;
		}
		return LoadTexture(pDevice, pTextureFilename);
	}

	bool DX11Model::Init(ID3D11Device* pDevice, const char* pModelFilename, const WCHAR* pTextureFilename)
	{
		// 모델 데이터를 로드합니다.
		if (!LoadVertices(pModelFilename))
		{
			return false;
		}

		// 정점 및 인덱스 버퍼를 초기화합니다.
		if (!InitBuffers(pDevice))
		{
			return false;
		}
		return LoadTexture(pDevice, pTextureFilename);
	}

	void DX11Model::Shutdown()
	{
		if (m_pTexture) { m_pTexture->Shutdown(); delete m_pTexture; m_pTexture = nullptr; }
		if (m_pIndexBuffer) { m_pIndexBuffer->Release();	m_pIndexBuffer = nullptr; }
		if (m_pVertexBuffer) { m_pVertexBuffer->Release(); m_pVertexBuffer = nullptr; }
		if (m_pVertices) { delete m_pVertices; m_pVertices = nullptr; }
	}

	void DX11Model::Render(ID3D11DeviceContext* pDeviceContext)
	{
		// 정점 버퍼의 단위와 오프셋을 설정합니다.
		UINT stride = sizeof(VertexType);
		UINT offset = 0;

		// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
		pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		// 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
		pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// 정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형으로 설정합니다.
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	bool DX11Model::InitBuffers(ID3D11Device* pDevice)
	{
		// 인덱스 배열을 만듭니다.
		uint32_t* indices = new uint32_t[m_vertexCount];

		// TODO: 추후 인덱스 배열 값을 불러오도록 구현
		// 인덱스 배열은 순차적으로 만듭니다.
		for (int i = 0; i < m_indexCount; i++)
		{
			indices[i] = i;
		}

		// 정적 정점 버퍼의 구조체를 설정합니다.
		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
		D3D11_SUBRESOURCE_DATA vertexData{};
		vertexData.pSysMem = m_pVertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// 이제 정점 버퍼를 만듭니다.
		if (FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer)))
		{
			return false;
		}

		// 정적 인덱스 버퍼의 구조체를 설정합니다.
		D3D11_BUFFER_DESC indexBufferDesc{};
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성합니다.
		D3D11_SUBRESOURCE_DATA indexData{};
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// 인덱스 버퍼를 생성합니다.
		if (FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer)))
		{
			return false;
		}

		// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제합니다.
		delete[] m_pVertices;
		m_pVertices = 0;

		delete[] indices;
		indices = 0;

		return true;
	}

	bool DX11Model::LoadVertices(const char* pFilename)
	{
		// 모델 파일을 엽니다.
		ifstream fin;
		fin.open(pFilename);

		// 파일을 열 수 없으면 종료합니다.
		if (fin.fail())
		{
			return false;
		}

		// 버텍스 카운트의 값까지 읽습니다.
		char input = 0;
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}

		// 버텍스 카운트를 읽습니다.
		fin >> m_vertexCount;

		// 인덱스의 수를 정점 수와 같게 설정합니다.
		m_indexCount = m_vertexCount;

		// 읽어 들인 정점 개수를 사용하여 모델을 만듭니다.
		m_pVertices = new VertexType[m_vertexCount];
		if (!m_pVertices)
		{
			return false;
		}

		// 데이터의 시작 부분까지 읽습니다.
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		// 버텍스 데이터를 읽습니다.
		for (int i = 0; i < m_vertexCount; i++)
		{
			fin >> m_pVertices[i].position.x >> m_pVertices[i].position.y >> m_pVertices[i].position.z;
			fin >> m_pVertices[i].texture.x >> m_pVertices[i].texture.y;
			fin >> m_pVertices[i].normal.x >> m_pVertices[i].normal.y >> m_pVertices[i].normal.z;
		}

		// 모델 파일을 닫습니다.
		fin.close();

		return true;
	}

	bool DX11Model::LoadTexture(ID3D11Device* pDevice, const WCHAR* pFilename)
	{
		// 텍스처 오브젝트를 생성한다.
		m_pTexture = new DX11Texture;
		if (!m_pTexture)
		{
			return false;
		}

		// 텍스처 오브젝트를 초기화한다.
		return m_pTexture->Init(pDevice, pFilename);
	}
	ID3D11ShaderResourceView* DX11Model::GetTexture()
	{
		return m_pTexture->GetTexture();
	}
}
