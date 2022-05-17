#include "kepch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Dependencies/stbimage/stb_image.h"

#include "DX11Texture.h"
#include "DX11Context.h"
#include "DDSTextureLoader.h"

namespace kepler {

	

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
		// 텍스처를 파일로부터 읽어온다
		if (FAILED(CreateDDSTextureFromFile(pDevice, pFilename, nullptr, &m_pTexture)))
		{
			return false;
		}

		return true;
	}

	void DX11Texture::Shutdown()
	{
		//텍스처 뷰 리소스를 해제한다.
		if (m_pTexture) { m_pTexture->Release(); m_pTexture = nullptr; }
	}


	//----------------------------------------------------------------
	
	// 나중에 더 다듬어서 사용
	static DXGI_FORMAT GetDXGIFormatFromInfo(const eTextureDataType type, const uint8_t channel, const uint8_t bytePerTexel);
	 
	// DX11Texture2D
	DX11Texture2D::DX11Texture2D(const eTextureDataType type, const uint32_t width, const uint32_t height, const uint8_t channel, const uint8_t bytePerTexel)
		: m_width{ width }
		, m_height{ height }
		, m_pResourceView{ nullptr }
		, m_pTexture{ nullptr }
		, m_pSamplerState{ nullptr }
	{
		//...
	}

	DX11Texture2D::DX11Texture2D(const eTextureDataType type, const std::string& filepath)
		: m_width{ 0 }
		, m_height{ 0 }
		, m_pResourceView{ nullptr }
		, m_pTexture{ nullptr }
		, m_pSamplerState{ nullptr }
	{
		int channel = 0;
		unsigned char* pRawImage = stbi_load(filepath.c_str(), reinterpret_cast<int*>(&m_width), reinterpret_cast<int*>(&m_height), &channel, 4);
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
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = pRawImage;
		initData.SysMemPitch = m_width * 4;

		ID3D11Device* pDevice;
		GetDX11DeviceAndDeviceContext(&pDevice, nullptr);

		// 텍스처 생성
		HRESULT hr = pDevice->CreateTexture2D(&texDesc, &initData, &m_pTexture);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to create texture");
			stbi_image_free(pRawImage);
			pRawImage = nullptr;
			return;
		}
		if (pRawImage)
		{
			stbi_image_free(pRawImage);
			pRawImage = nullptr;
		}

		// 리소스 뷰 생성
		hr = pDevice->CreateShaderResourceView(m_pTexture, nullptr, &m_pResourceView);

		// 샘플러 생성
		D3D11_SAMPLER_DESC samplerDesc{};
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	}

	void DX11Texture2D::Bind(uint32_t slot)
	{
		ID3D11DeviceContext* pDeviceContext = nullptr;
		GetDX11DeviceAndDeviceContext(nullptr, &pDeviceContext);

		pDeviceContext->PSSetShaderResources(slot, 1, &m_pResourceView);
		pDeviceContext->PSSetSamplers(slot, 1, &m_pSamplerState);
	}

	void DX11Texture2D::SetData(const void* data, const uint32_t size)
	{
		//TODO: 추후 구현
	}


	// 나중에 더 다듬어서 사용
	[[deprecated]]
	DXGI_FORMAT GetDXGIFormatFromInfo(const eTextureDataType type, const uint8_t channel, const uint8_t bytePerTexel)
	{
		uint8_t elemSize = bytePerTexel / channel;

		switch (type)
		{
		case eTextureDataType::Typeless:
			{
				switch (channel)
				{
				case 1:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8_TYPELESS;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16_TYPELESS;
					else if (bytePerTexel == 4) return DXGI_FORMAT_R32_TYPELESS;
				case 2:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8G8_TYPELESS;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16G16_TYPELESS;
					else if (bytePerTexel == 4) return DXGI_FORMAT_R32G32_TYPELESS;
				case 4:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8G8B8A8_TYPELESS;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16G16B16A16_TYPELESS;
					else if (bytePerTexel == 4) return DXGI_FORMAT_R32G32B32A32_TYPELESS;
				}
			}
			break;
		case eTextureDataType::Int:
			{
				switch (channel)
				{
				case 1:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8_SINT;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16_SINT;
					else if (bytePerTexel == 4) return DXGI_FORMAT_R32_SINT;
				case 2:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8G8_SINT;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16G16_SINT;
					else if (bytePerTexel == 4) return DXGI_FORMAT_R32G32_SINT;
				case 4:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8G8B8A8_SINT;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16G16B16A16_SINT;
					else if (bytePerTexel == 4) return DXGI_FORMAT_R32G32B32A32_SINT;
				}
			}
			break;
		case eTextureDataType::UInt:
			{
				switch (channel)
				{
				case 1:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8_UINT;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16_UINT;
					else if (bytePerTexel == 4) return DXGI_FORMAT_R32_UINT;
				case 2:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8G8_UINT;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16G16_UINT;
					else if (bytePerTexel == 4) return DXGI_FORMAT_R32G32_UINT;
				case 4:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8G8B8A8_UINT;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16G16B16A16_UINT;
					else if (bytePerTexel == 4) return DXGI_FORMAT_R32G32B32A32_UINT;
				}
			}
			break;
		case eTextureDataType::Float:
			{
				switch (channel)
				{
				case 1:
					if		(bytePerTexel == 2) return DXGI_FORMAT_R16_FLOAT;
					else if (bytePerTexel == 4) return DXGI_FORMAT_R32_FLOAT;
				case 2:
					if		(bytePerTexel == 2) return DXGI_FORMAT_R16G16_FLOAT;
					else if (bytePerTexel == 4) return DXGI_FORMAT_R32G32_FLOAT;
				case 4:
					if		(bytePerTexel == 2) return DXGI_FORMAT_R16G16B16A16_FLOAT;
					else if (bytePerTexel == 4) return DXGI_FORMAT_R32G32B32A32_FLOAT;
				}
			}
			break;
		case eTextureDataType::NormalizedInt:
			{
				switch (channel)
				{
				case 1:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8_SNORM;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16_SNORM;
				case 2:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8G8_SNORM;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16G16_SNORM;
				case 4:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8G8B8A8_SNORM;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16G16B16A16_SNORM;
				}
			}
			break;
		case eTextureDataType::NormalizedUInt:
			{
				switch (channel)
				{
				case 1:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8_UNORM;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16_UNORM;
				case 2:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8G8_UNORM;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16G16_TYPELESS;
				case 4:
					if		(bytePerTexel == 1) return DXGI_FORMAT_R8G8B8A8_UNORM;
					else if (bytePerTexel == 2) return DXGI_FORMAT_R16G16B16A16_TYPELESS;
				}
			}
			break;
		}
		KEPLER_ASSERT(false, "Invalid texture format");
		return DXGI_FORMAT_UNKNOWN;
	}
}