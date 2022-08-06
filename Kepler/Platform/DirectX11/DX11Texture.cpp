#include "kepch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Dependencies/stbimage/stb_image.h"

#include "DX11Texture.h"
#include "DX11Context.h"

namespace kepler {

	// DX11Texture2D
	DX11Texture2D::DX11Texture2D(const eTextureDataType type, const uint32_t width, const uint32_t height, const uint8_t channel, const uint8_t bytePerTexel)
		: m_width{ width }
		, m_height{ height }
		, m_pResourceView{ nullptr }
		, m_pTexture{ nullptr }
	{
		// 임시 텍스처 혹은 렌더링 정보 저장용으로 주로 사용될 수 있음.
		D3D11_TEXTURE2D_DESC texDesc{};
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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

	DX11Texture2D::DX11Texture2D(const eTextureDataType type, const std::string& filepath)
		: m_width{ 0 }
		, m_height{ 0 }
		, m_pResourceView{ nullptr }
		, m_pTexture{ nullptr }
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
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
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

		// 텍스처 생성
		HRESULT hr = pDevice->CreateTexture2D(&texDesc, nullptr, &m_pTexture);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to create texture");
			return;
		}

		// 텍스처 리소스 설정 및 raw image 해제
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

		// 리소스 뷰 생성
		hr = pDevice->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pResourceView);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to create Shader Resource View");
		}
		
		// mipmap 생성
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

	void DX11Texture2D::SetData(const void* data, const uint32_t size)
	{
		//TODO: 추후 구현
	}
}