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
		// ���� �����͸� �޽��ϴ�.
		m_pVertices = pVertices;
		m_vertexCount = vertex_count;
		m_indexCount = m_vertexCount;

		// ���� �� �ε��� ���۸� �ʱ�ȭ�մϴ�.
		if (!InitBuffers(pDevice))
		{
			return false;
		}
		return LoadTexture(pDevice, pTextureFilename);
	}

	bool DX11Model::Init(ID3D11Device* pDevice, const char* pModelFilename, const WCHAR* pTextureFilename)
	{
		// �� �����͸� �ε��մϴ�.
		if (!LoadVertices(pModelFilename))
		{
			return false;
		}

		// ���� �� �ε��� ���۸� �ʱ�ȭ�մϴ�.
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
		// ���� ������ ������ �������� �����մϴ�.
		UINT stride = sizeof(VertexType);
		UINT offset = 0;

		// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ �����մϴ�.
		pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ �����մϴ�.
		pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ �����մϴ�.
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	bool DX11Model::InitBuffers(ID3D11Device* pDevice)
	{
		// �ε��� �迭�� ����ϴ�.
		uint32_t* indices = new uint32_t[m_vertexCount];

		// TODO: ���� �ε��� �迭 ���� �ҷ������� ����
		// �ε��� �迭�� ���������� ����ϴ�.
		for (int i = 0; i < m_indexCount; i++)
		{
			indices[i] = i;
		}

		// ���� ���� ������ ����ü�� �����մϴ�.
		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
		D3D11_SUBRESOURCE_DATA vertexData{};
		vertexData.pSysMem = m_pVertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// ���� ���� ���۸� ����ϴ�.
		if (FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer)))
		{
			return false;
		}

		// ���� �ε��� ������ ����ü�� �����մϴ�.
		D3D11_BUFFER_DESC indexBufferDesc{};
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
		D3D11_SUBRESOURCE_DATA indexData{};
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// �ε��� ���۸� �����մϴ�.
		if (FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer)))
		{
			return false;
		}

		// �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� �����մϴ�.
		delete[] m_pVertices;
		m_pVertices = 0;

		delete[] indices;
		indices = 0;

		return true;
	}

	bool DX11Model::LoadVertices(const char* pFilename)
	{
		// �� ������ ���ϴ�.
		ifstream fin;
		fin.open(pFilename);

		// ������ �� �� ������ �����մϴ�.
		if (fin.fail())
		{
			return false;
		}

		// ���ؽ� ī��Ʈ�� ������ �н��ϴ�.
		char input = 0;
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}

		// ���ؽ� ī��Ʈ�� �н��ϴ�.
		fin >> m_vertexCount;

		// �ε����� ���� ���� ���� ���� �����մϴ�.
		m_indexCount = m_vertexCount;

		// �о� ���� ���� ������ ����Ͽ� ���� ����ϴ�.
		m_pVertices = new VertexType[m_vertexCount];
		if (!m_pVertices)
		{
			return false;
		}

		// �������� ���� �κб��� �н��ϴ�.
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		// ���ؽ� �����͸� �н��ϴ�.
		for (int i = 0; i < m_vertexCount; i++)
		{
			fin >> m_pVertices[i].position.x >> m_pVertices[i].position.y >> m_pVertices[i].position.z;
			fin >> m_pVertices[i].texture.x >> m_pVertices[i].texture.y;
			fin >> m_pVertices[i].normal.x >> m_pVertices[i].normal.y >> m_pVertices[i].normal.z;
		}

		// �� ������ �ݽ��ϴ�.
		fin.close();

		return true;
	}

	bool DX11Model::LoadTexture(ID3D11Device* pDevice, const WCHAR* pFilename)
	{
		// �ؽ�ó ������Ʈ�� �����Ѵ�.
		m_pTexture = new DX11Texture;
		if (!m_pTexture)
		{
			return false;
		}

		// �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�.
		return m_pTexture->Init(pDevice, pFilename);
	}
	ID3D11ShaderResourceView* DX11Model::GetTexture()
	{
		return m_pTexture->GetTexture();
	}
}
