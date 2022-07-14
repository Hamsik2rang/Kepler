#include "kepch.h"

#include "DX11FrameBuffer.h"
#include "DX11Context.h"
#include "Core/Application.h"

namespace kepler {

	DX11FrameBuffer::DX11FrameBuffer()
		: m_pDepthStencilView{ nullptr }
		, m_pRenderTargetViews{ nullptr }
		, m_pTextures{ nullptr }
		, m_pShaderResourceViews{ nullptr }
	{
		Init();
	}

	DX11FrameBuffer::~DX11FrameBuffer()
	{
		if (m_pDepthStencilView)
		{
			m_pDepthStencilView->Release();
			m_pDepthStencilView = nullptr;
		}

		if (m_pColorBufferView)
		{
			m_pColorBufferView->Release();
			m_pColorBufferView = nullptr;
		}

		for (auto& rtv : m_pRenderTargetViews)
		{
			if (rtv)
			{
				rtv->Release();
				rtv = nullptr;
			}
		}

		for (auto& srv : m_pShaderResourceViews)
		{
			if (srv)
			{
				srv->Release();
				srv = nullptr;
			}
		}

		for (auto& texture : m_pTextures)
		{
			if (texture)
			{
				texture->Release();
				texture = nullptr;
			}
		}
	}

	void DX11FrameBuffer::Init()
	{
		// Create Color Buffer(Main Render Target)
		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		IDXGISwapChain* pSwapChain = IGraphicsContext::Get()->GetSwapChain();

		ID3D11Texture2D* pBackBuffer = nullptr;
		HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to Get Back Buffer from SwapChain.");
			KEPLER_CORE_CRITICAL("Can't Create Color Buffer");
			return;
		}

		pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pColorBufferView);
		pBackBuffer->Release();
		pBackBuffer = nullptr;
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to Create Color Buffer.");
			KEPLER_CORE_CRITICAL("Can't Create Color Buffer");
			return;
		}

		// Create Depth Stencil View
		IWindow* pWindow = Application::Get()->GetWindow();
		uint32_t width = pWindow->GetWidth();
		uint32_t height = pWindow->GetHeight();

		D3D11_TEXTURE2D_DESC dsDesc{};
		dsDesc.Width = width;
		dsDesc.Height = height;
		dsDesc.MipLevels = 1;
		dsDesc.ArraySize = 1;
		dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsDesc.SampleDesc.Count = 1;
		dsDesc.SampleDesc.Quality = 0;
		dsDesc.Usage = D3D11_USAGE_DEFAULT;
		dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsDesc.CPUAccessFlags = 0;
		dsDesc.MiscFlags = 0;

		ID3D11Texture2D* pDepthStencilBuffer = nullptr;
		hr = pDevice->CreateTexture2D(&dsDesc, nullptr, &pDepthStencilBuffer);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to create Depth Stencil Buffer.");
			KEPLER_CORE_CRITICAL("Cannot Create Depth Stencil Buffer.");
			return;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format = dsDesc.Format;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		hr = pDevice->CreateDepthStencilView(pDepthStencilBuffer, &dsvDesc, &m_pDepthStencilView);
		//TODO: DepthStencilBuffer가 릴리즈 되면 안될 시 멤버 변수로 변경할 것
		pDepthStencilBuffer->Release();
		pDepthStencilBuffer = nullptr;
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to create Depth Stencil View");
			KEPLER_CORE_CRITICAL("Cannot Create Depth Stencil View.");
			return;
		}

		// Set Color Buffer & Depth Stencil Buffer
		pContext->OMSetRenderTargets(1, &m_pColorBufferView, m_pDepthStencilView);
	}

	void DX11FrameBuffer::ClearColor(const float color[4])
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		pContext->ClearRenderTargetView(m_pColorBufferView, color);
	}

	void DX11FrameBuffer::ClearDepthStencil(bool bDepthClear, bool bStencilClear, const float depth, const uint8_t stencil)
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		uint32_t clearFlag = 0;
		if (bDepthClear)
		{
			clearFlag |= D3D11_CLEAR_DEPTH;
		}
		if (bStencilClear)
		{
			clearFlag |= D3D11_CLEAR_STENCIL;
		}

		pContext->ClearDepthStencilView(m_pDepthStencilView, clearFlag, depth, stencil);
	}

	void DX11FrameBuffer::ClearGBuffer(const float color[4])
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		for (auto& rtv : m_pRenderTargetViews)
		{
			pContext->ClearRenderTargetView(rtv, color);
		}
	}

	void DX11FrameBuffer::ClearGBuffer(uint8_t index, const float color[4])
	{
		KEPLER_CORE_ASSERT(index < 8, "G-Buffer index range is [0, 8).");

		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		pContext->ClearRenderTargetView(m_pRenderTargetViews[index], color);
	}

	void DX11FrameBuffer::ClearGBuffer(uint8_t startSlot, uint8_t count, const float color[4])
	{
		KEPLER_CORE_ASSERT(startSlot + count < 8, "Index of G-Buffer cannot larger than 7.");

		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		for (uint8_t i = startSlot; i < startSlot + count; i++)
		{
			pContext->ClearRenderTargetView(m_pRenderTargetViews[i], color);
		}
	}

	void DX11FrameBuffer::BindColorBuffer()
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		pContext->OMSetRenderTargets(1, &m_pColorBufferView, m_pDepthStencilView);
	}

	void DX11FrameBuffer::UnbindColorBuffer()
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		pContext->OMSetRenderTargets(1, nullptr, nullptr);
	}

	void DX11FrameBuffer::BindGBuffer(uint8_t startSlot, uint8_t count)
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		pContext->OMSetRenderTargets(8, m_pRenderTargetViews, m_pDepthStencilView);
	}

	void DX11FrameBuffer::UnbindGBuffer(uint8_t startSlot, uint8_t count)
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		pContext->OMSetRenderTargets(1, nullptr, nullptr);
	}

	void DX11FrameBuffer::AddGBuffer(uint8_t startSlot, uint8_t count)
	{
		KEPLER_CORE_ASSERT(startSlot < 8, "G-Buffer index range is [0, 8).");
		KEPLER_CORE_ASSERT(startSlot + count < 8, "Index of G-Buffer cannot larger than 7.");
		KEPLER_CORE_ASSERT(count < 8, "Maximum G-Buffer available count is 8.");

		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		IWindow* pWindow = Application::Get()->GetWindow();
		uint32_t width = pWindow->GetWidth();
		uint32_t height = pWindow->GetHeight();

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

		for (uint8_t i = startSlot; i < startSlot + count; i++)
		{
			HRESULT hr = pDevice->CreateTexture2D(&texDesc, nullptr, &m_pTextures[i]);
			if (FAILED(hr))
			{
				KEPLER_CORE_ASSERT(false, "Fail to Create Texture for G-Buffer.");
				break;
			}
		}

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = texDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		for (uint8_t i = startSlot; i < startSlot + count; i++)
		{
			HRESULT hr = pDevice->CreateRenderTargetView(m_pTextures[i], &rtvDesc, &m_pRenderTargetViews[i]);
			if (FAILED(hr))
			{
				KEPLER_CORE_ASSERT(false, "Fail to Create Render Target for G-Buffer.");
				break;
			}
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		for (uint8_t i = startSlot; i < startSlot + count; i++)
		{
			HRESULT hr = pDevice->CreateShaderResourceView(m_pTextures[i], &srvDesc, &m_pShaderResourceViews[i]);
			if (FAILED(hr))
			{
				KEPLER_CORE_ASSERT(false, "Fail to Create Shader Resource View for G-Buffer.");
				break;
			}
		}
	}

	void DX11FrameBuffer::DeleteGBuffer(uint8_t startSlot, uint8_t count)
	{
		KEPLER_CORE_ASSERT(startSlot < 8, "G-Buffer index range is [0, 8).");
		KEPLER_CORE_ASSERT(startSlot + count < 8, "Index of G-Buffer cannot larger than 7.");
		KEPLER_CORE_ASSERT(count < 8, "Maximum G-Buffer available count is 8.");

		for (uint8_t i = startSlot; i < startSlot + count; i++)
		{
			if (m_pRenderTargetViews[i])
			{
				m_pRenderTargetViews[i]->Release();
				m_pRenderTargetViews[i] = nullptr;
			}
			if (m_pTextures[i])
			{
				m_pTextures[i]->Release();
				m_pTextures[i] = nullptr;
			}
			if (m_pShaderResourceViews[i])
			{
				m_pShaderResourceViews[i]->Release();
				m_pShaderResourceViews[i] = nullptr;
			}
		}
	}
}