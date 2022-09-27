#include "kepch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"

#include "DX11Texture.h"
#include "DX11Context.h"


namespace kepler {

	static DXGI_FORMAT ConvertTextureDataType(const ETextureDataType type);

	// DX11Texture2D
	DX11Texture2D::DX11Texture2D(const ETextureDataType type, const uint32_t width, const uint32_t height, const uint8_t channel, const uint8_t bytePerTexel)
		: m_pResourceView{ nullptr }
		, m_pTexture{ nullptr }
		, m_type{ type }
		, m_width{ width }
		, m_height{ height }
	{
		// �ӽ� �ؽ�ó Ȥ�� ������ ���� ��������� �ַ� ���� �� ����.
		D3D11_TEXTURE2D_DESC texDesc{};
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = ConvertTextureDataType(type);
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.CPUAccessFlags = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();

		HRESULT hr = pDevice->CreateTexture2D(&texDesc, nullptr, &m_pTexture);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to create texture");
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		hr = pDevice->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pResourceView);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to create DX11 shader resource view");
		}
	}

	DX11Texture2D::DX11Texture2D(const ETextureDataType type, const std::string& filepath)
		: m_pResourceView{ nullptr }
		, m_pTexture{ nullptr }
		, m_type{ type }
		, m_width{ 0 }
		, m_height{ 0 }
	{
		int channel = 0;
		unsigned char* pRawImage = stbi_load(filepath.c_str(), reinterpret_cast<int*>(&m_width), reinterpret_cast<int*>(&m_height), &channel, 0);
		if (!pRawImage)
		{
			KEPLER_CORE_ASSERT(false, "Fail to load ImageFile");
			return;
		}

		// Temporary
		D3D11_TEXTURE2D_DESC texDesc{};
		texDesc.Width = m_width;
		texDesc.Height = m_height;
		texDesc.Format = ConvertTextureDataType(type);
		texDesc.MipLevels = 0;
		texDesc.ArraySize = 1;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		texDesc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();

		// �ؽ�ó ����
		HRESULT hr = pDevice->CreateTexture2D(&texDesc, nullptr, &m_pTexture);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to create texture");
			return;
		}

		// �ؽ�ó ���ҽ� ���� �� raw image ����
		uint32_t pitch = m_width * 4;
		pDeviceContext->UpdateSubresource(m_pTexture, 0, nullptr, pRawImage, pitch, 0);
		if (pRawImage)
		{
			stbi_image_free(pRawImage);
			pRawImage = nullptr;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		// ���ҽ� �� ����
		hr = pDevice->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pResourceView);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to create Shader Resource View");
		}

		// mipmap ����
		pDeviceContext->GenerateMips(m_pResourceView);
	}

	DX11Texture2D::~DX11Texture2D()
	{
		if (m_pTexture)
		{
			m_pTexture->Release();
			m_pTexture = nullptr;
		}
		if (m_pResourceView)
		{
			m_pResourceView->Release();
			m_pResourceView = nullptr;
		}
	}

	void DX11Texture2D::Bind(const uint32_t slot)
	{
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();
		pDeviceContext->PSSetShaderResources(slot, 1, &m_pResourceView);
	}

	void DX11Texture2D::SetData(const void* pData, const uint32_t width, const uint32_t height)
	{
		KEPLER_CORE_ASSERT(width == m_width && height == m_height, "Texture data size is not same with texture view");

		m_width = width;
		m_height = height;

		// TODO: Access Violance
		uint32_t pitch = m_width * 4;
		auto pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();
		pDeviceContext->UpdateSubresource(m_pTexture, 0, nullptr, pData, pitch, 0);
	}

	DXGI_FORMAT ConvertTextureDataType(const ETextureDataType type)
	{
		switch (type)
		{
		case ETextureDataType::Float_RGBA16:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case ETextureDataType::Float_RGBA32:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ETextureDataType::Float_RGB32:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case ETextureDataType::UNorm_RGBA8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			//...
		}
		return DXGI_FORMAT_UNKNOWN;
	}
}