#include "kepch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"

#include "DX11Texture.h"
#include "DX11Context.h"


namespace kepler {

static DXGI_FORMAT ConvertTextureDataType(const eTextureDataType type);
static uint8_t GetChannelFromTextureDataType(const eTextureDataType type);

// DX11Texture2D
DX11Texture2D::DX11Texture2D(const eTextureDataType type, const uint32_t width, const uint32_t height)
	: m_pResourceView{ nullptr }
	, m_pTexture{ nullptr }
	, m_type{ type }
	, m_width{ width }
	, m_height{ height }
	, m_channel{ GetChannelFromTextureDataType(type) }
{
	// 임시 텍스처 혹은 렌더링 정보 저장용으로 주로 사용될 수 있음.
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

	ID3D11Device* pDevice = static_cast<ID3D11Device*>(IGraphicsContext::Get()->GetDevice());

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
	: m_pResourceView{ nullptr }
	, m_pTexture{ nullptr }
	, m_type{ type }
	, m_width{ 0 }
	, m_height{ 0 }
	, m_channel{ GetChannelFromTextureDataType(type) }
{
	unsigned char* pRawImage = stbi_load(filepath.c_str(), reinterpret_cast<int*>(&m_width), reinterpret_cast<int*>(&m_height), reinterpret_cast<int*>(&m_channel), 0);
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

	ID3D11Device* pDevice = static_cast<ID3D11Device*>(IGraphicsContext::Get()->GetDevice());
	ID3D11DeviceContext* pDeviceContext = static_cast<ID3D11DeviceContext*>(IGraphicsContext::Get()->GetDeviceContext());

	// 텍스처 생성
	HRESULT hr = pDevice->CreateTexture2D(&texDesc, nullptr, &m_pTexture);
	if (FAILED(hr))
	{
		KEPLER_CORE_ASSERT(false, "Fail to create texture");
		return;
	}

	// 텍스처 리소스 설정 및 raw image 해제
	SetData(pRawImage, m_width, m_height, m_channel);
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
	ID3D11DeviceContext* pDeviceContext = static_cast<ID3D11DeviceContext*>(IGraphicsContext::Get()->GetDeviceContext());
	pDeviceContext->PSSetShaderResources(slot, 1, &m_pResourceView);
}


//TODO: (Im Yongsik) 현재 UNormRGBA8 타입 데이터만 씌울 수 있도록 되어 있습니다. 개선이 필요합니다.
void DX11Texture2D::SetData(const void* pData, const uint32_t width, const uint32_t height, const uint32_t channel)
{
	KEPLER_CORE_ASSERT(width == m_width && height == m_height, "Texture data size is not same with texture view");

	// (Im Yongsik) DX11은 3채널을 지원하지 않으므로 4채널로 전환해야 함
	if (channel == 3)
	{
		uint8_t RGBASize = 4;
		m_channel = RGBASize;
		uint8_t* pBuffer = new uint8_t[width * height * RGBASize];
		for (int row = 0; row < height; row++)
		{
			for (int bufCol = 0, dataCol = 0; dataCol < width * channel; bufCol += RGBASize, dataCol += channel)
			{
				auto bufIndex = width * RGBASize * row + bufCol;
				auto dataIndex = width * channel * row + dataCol;

				pBuffer[bufIndex + 0] = ((uint8_t*)pData)[dataIndex + 0];
				pBuffer[bufIndex + 1] = ((uint8_t*)pData)[dataIndex + 1];
				pBuffer[bufIndex + 2] = ((uint8_t*)pData)[dataIndex + 2];
				pBuffer[bufIndex + 3] = 255;
			}
		}
		ID3D11DeviceContext* pDeviceContext = static_cast<ID3D11DeviceContext*>(IGraphicsContext::Get()->GetDeviceContext());
		uint32_t pitch = RGBASize * width;
		pDeviceContext->UpdateSubresource(m_pTexture, 0, nullptr, pBuffer, RGBASize * width, 0);
		delete[] pBuffer;
	}
	else
	{
		uint32_t pitch = m_width * channel;
		ID3D11DeviceContext* pDeviceContext = static_cast<ID3D11DeviceContext*>(IGraphicsContext::Get()->GetDeviceContext());
		pDeviceContext->UpdateSubresource(m_pTexture, 0, nullptr, pData, pitch, 0);
	}
}

DXGI_FORMAT ConvertTextureDataType(const eTextureDataType type)
{
	switch (type)
	{
	case eTextureDataType::Float_RGBA16:
		return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case eTextureDataType::Float_RGBA32:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case eTextureDataType::Float_RGB32:
		return DXGI_FORMAT_R32G32B32_FLOAT;
	case eTextureDataType::UNorm_RGBA8:
	case eTextureDataType::UNorm_RGB8:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
		//...
	}
	return DXGI_FORMAT_UNKNOWN;
}

uint8_t GetChannelFromTextureDataType(const eTextureDataType type)
{
	switch (type)
	{
	case eTextureDataType::Float_RGBA16:
	case eTextureDataType::Float_RGBA32:
	case eTextureDataType::UNorm_RGBA8:
		return 4u;
	case eTextureDataType::Float_RGB32:
	case eTextureDataType::UNorm_RGB8:
		return 3u;
		//...
	}
	return 0u;
}

}