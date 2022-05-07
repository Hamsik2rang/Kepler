#include "kepch.h"

#include "DX11Texture.h"
#include "DDSTextureLoader.h"

namespace kepler {
	using namespace DirectX;

	DX11Texture::DX11Texture()
	{
	}

	DX11Texture::DX11Texture(const DX11Texture& other)
	{
	}

	DX11Texture::~DX11Texture()
	{
	}

	bool DX11Texture::Init(ID3D11Device* pDevice, const WCHAR* pFilename)
	{
		// �ؽ�ó�� ���Ϸκ��� �о�´�
		if (FAILED(CreateDDSTextureFromFile(pDevice, pFilename, nullptr, &m_pTexture)))
		{
			return false;
		}

		return true;
	}

	void DX11Texture::Shutdown()
	{
		//�ؽ�ó �� ���ҽ��� �����Ѵ�.
		if (m_pTexture) { m_pTexture->Release(); m_pTexture = nullptr; }
	}
}