#include "kepch.h"

#include "DX11State.h"
#include "Renderer/GraphicsContext.h"

namespace kepler {

	DX11State::DX11State()
		: m_desc{}
		, m_bHasChanged{ true }
		, m_pBlendState{ nullptr }
		, m_pDepthStencilState{ nullptr }
		, m_pRasterizerState{ nullptr }
	{
		m_desc.bCullBackFace = true;
		m_desc.bIsFrontClockwise = true;
		m_desc.bDepthTest = true;
		m_desc.bDepthWrite = false;
		m_desc.bWireFrame = false;
	}

	DX11State::~DX11State()
	{
		if (m_pRasterizerState)
		{
			m_pRasterizerState->Release();
			m_pRasterizerState = nullptr;
		}
		if (m_pBlendState)
		{
			m_pBlendState->Release();
			m_pBlendState = nullptr;
		}
		if (m_pDepthStencilState)
		{
			m_pDepthStencilState->Release();
			m_pDepthStencilState = nullptr;
		}
	}

	void DX11State::SetShaderState(ShaderStateDescription& desc)
	{
		s_shaderDesc = desc;
	}

	void DX11State::SetShaderState(const eShaderType& type, const std::shared_ptr<IShader>& pShader)
	{
		switch (type)
		{
		case eShaderType::Vertex:	s_shaderDesc.pVertexShader		= pShader; break;
		case eShaderType::Hull:		s_shaderDesc.pHullShader		= pShader; break;
		case eShaderType::Domain:	s_shaderDesc.pDomainShader		= pShader; break;
		case eShaderType::Geometry:	s_shaderDesc.pGeometryShader	= pShader; break;
		case eShaderType::Pixel:	s_shaderDesc.pPixelShader		= pShader; break;
		}
	}

	void DX11State::SetRasterizerState(RasterizerStateDescription& desc)
	{
		D3D11_RASTERIZER_DESC rsDesc{};

		if (desc.bCullBackFace) rsDesc.CullMode = D3D11_CULL_BACK;
		else					rsDesc.CullMode = D3D11_CULL_FRONT;

		if (desc.bIsFrontClockwise) rsDesc.FrontCounterClockwise = false;
		else						rsDesc.FrontCounterClockwise = true;

		if (desc.bWireFrame) rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		else				 rsDesc.FillMode = D3D11_FILL_SOLID;
	}

	void DX11State::SetDepthState(DepthStateDescription& desc)
	{

	}

	void DX11State::SetStencilState(StencilStateDescription& desc)
	{

	}

	void DX11State::SetBlendState(BlendStateDescription& desc)
	{


	}

	void DX11State::SetRasterizerState()
	{
		D3D11_RASTERIZER_DESC desc{};
		if (m_desc.bCullBackFace) desc.CullMode = D3D11_CULL_BACK;
		else desc.CullMode = D3D11_CULL_FRONT;

		if (m_desc.bIsFrontClockwise) desc.FrontCounterClockwise = false;
		else desc.FrontCounterClockwise = true;

		if (m_desc.bWireFrame) desc.FillMode = D3D11_FILL_WIREFRAME;
		else desc.FillMode = D3D11_FILL_SOLID;

		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();
		if (m_pRasterizerState)
		{
			m_pRasterizerState->Release();
			m_pRasterizerState = nullptr;
		}
		HRESULT hr = pDevice->CreateRasterizerState(&desc, &m_pRasterizerState);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to Create Rasterizer State");
			return;
		}
		pDeviceContext->RSSetState(m_pRasterizerState);
	}

	void DX11State::SetBlendState()
	{
		D3D11_BLEND_DESC desc{};
		if (m_desc.bBlendEnable) desc.RenderTarget->BlendEnable = true;
		else desc.RenderTarget->BlendEnable = false;
		//...
	}

	void DX11State::SetDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC desc{};
		if (m_desc.bDepthTest) desc.DepthEnable = true;
		else desc.DepthEnable = false;





		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();

	}

	void DX11State::Bind()
	{
		if (m_bHasChanged)
		{
			//...
			SetRasterizerState();
			SetBlendState();
			SetDepthStencilState();

			m_bHasChanged = false;
		}
	}

}