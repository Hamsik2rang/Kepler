#include "kepch.h"

#include "DX11State.h"
#include "Renderer/GraphicsContext.h"

namespace kepler {

	DX11State::DX11State()
		: m_pBlendState{ nullptr }
		, m_pDepthStencilState{ nullptr }
		, m_pRasterizerState{ nullptr }
		, m_bBlendStateChaged{ false }
		, m_bRasterizerStateChanged{ false }
		, m_bDepthStateChanged{ false }
	{

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

	void DX11State::SetShaderState(const eShaderType& type, const std::shared_ptr<IShader>& pShader)
	{
		switch (type)
		{
		case eShaderType::Vertex:	s_shaderDesc.pVertexShader = pShader;	break;
		case eShaderType::Hull:		s_shaderDesc.pHullShader = pShader;		break;
		case eShaderType::Domain:	s_shaderDesc.pDomainShader = pShader;	break;
		case eShaderType::Geometry:	s_shaderDesc.pGeometryShader = pShader; break;
		case eShaderType::Pixel:	s_shaderDesc.pPixelShader = pShader;	break;
		}
	}

	void DX11State::CreateRasterizerState()
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		// 이미 파이프라인에 들어가 있는 기존 State 제거
		if (m_pRasterizerState)
		{
			pContext->RSSetState(nullptr);
			m_pRasterizerState->Release();
			m_pRasterizerState = nullptr;
		}

		D3D11_RASTERIZER_DESC desc{};

		if (s_rasterizerDesc.bCullBackFace)	desc.CullMode = D3D11_CULL_BACK;
		else								desc.CullMode = D3D11_CULL_FRONT;

		if (s_rasterizerDesc.bWireFrame)	desc.FillMode = D3D11_FILL_WIREFRAME;
		else								desc.FillMode = D3D11_FILL_SOLID;

		//...
		desc.FrontCounterClockwise = !s_rasterizerDesc.bIsFrontClockwise;
		desc.DepthBias = s_rasterizerDesc.depthBias;
		desc.DepthBiasClamp = s_rasterizerDesc.depthClampBias;
		desc.DepthClipEnable = s_rasterizerDesc.bDepthClip;
		desc.ScissorEnable = s_rasterizerDesc.bScissssor;
		desc.AntialiasedLineEnable = s_rasterizerDesc.bAntiAliasedLine;
		desc.MultisampleEnable = s_rasterizerDesc.bMultiSampling;
		//desc.SlopeScaledDepthBias = s_rasterizerDesc.slo

	}

	void DX11State::CreateDepthStencilState()
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		// 이미 파이프라인에 들어가 있는 기존 State 제거
		if (m_pDepthStencilState)
		{
			pContext->OMSetDepthStencilState(nullptr, 0x00);
			m_pDepthStencilState->Release();
			m_pDepthStencilState = nullptr;
		}

		D3D11_DEPTH_STENCIL_DESC desc{};
		m_pDepthStencilState->GetDesc(&desc);

		if (s_depthDesc.bDepthTest)	desc.DepthEnable = true;
		else						desc.DepthEnable = false;

		if (s_depthDesc.bDepthWrite) desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		else						 desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		switch (s_depthDesc.comparer)
		{
		case eDepthComparer::Always:			desc.DepthFunc = D3D11_COMPARISON_ALWAYS;			break;
		case eDepthComparer::Never:				desc.DepthFunc = D3D11_COMPARISON_NEVER;			break;
		case eDepthComparer::Equal:				desc.DepthFunc = D3D11_COMPARISON_EQUAL;			break;
		case eDepthComparer::NotEqual:			desc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;		break;
		case eDepthComparer::Greater:			desc.DepthFunc = D3D11_COMPARISON_GREATER;			break;
		case eDepthComparer::GreaterOrEqual:	desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;	break;
		case eDepthComparer::Less:				desc.DepthFunc = D3D11_COMPARISON_LESS;				break;
		case eDepthComparer::LessOrEqual:		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;		break;
		case eDepthComparer::None:
			{
				KEPLER_CORE_ASSERT(desc.DepthEnable == true, "None Depth Comparer is not supported when Depth test is enabled");
			}
			break;
		}

		//...
	}

	void DX11State::CreateBlendState()
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		// 이미 파이프라인에 들어가 있는 기존 State 제거
		if (m_pBlendState)
		{
			pContext->OMSetBlendState(nullptr, nullptr, 0x00);
			m_pBlendState->Release();
			m_pBlendState = nullptr;
		}
		D3D11_BLEND_DESC desc{};

		//...
	}

	void DX11State::Bind()
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		if (m_bShaderStateChaged)
		{
			// Shader Binding을 여기서 하는 게 좋을까? 아니면 각각의 쉐이더를 원하는 지점에서 ShaderCache로 불러와 직접 Bind하는 게 좋을까?
			// Bind Shader
			if (s_shaderDesc.pVertexShader)		s_shaderDesc.pVertexShader->Bind();
			if (s_shaderDesc.pHullShader)		s_shaderDesc.pHullShader->Bind();
			if (s_shaderDesc.pDomainShader)		s_shaderDesc.pDomainShader->Bind();
			if (s_shaderDesc.pGeometryShader)	s_shaderDesc.pDomainShader->Bind();
			if (s_shaderDesc.pPixelShader)		s_shaderDesc.pPixelShader->Bind();

			m_bShaderStateChaged = false;
		}
		if (m_bRasterizerStateChanged)
		{
			CreateRasterizerState();
			pContext->RSSetState(m_pRasterizerState);

			m_bRasterizerStateChanged = false;
		}
		if (m_bDepthStateChanged || m_bStencilStateChaged)
		{
			CreateDepthStencilState();
			// NOTE: Stencil test sudo code:
			// if ( StencilRef & StencilReadMask StencilOP(Front/Back) Value & StencilReadMask)
			// TODO: 적절한 StencilRef를 넘기기 위한 방법 강구하기
			pContext->OMSetDepthStencilState(m_pDepthStencilState, 0xff);

			m_bDepthStateChanged = false;
			m_bStencilStateChaged = false;
		}
		if (m_bBlendStateChaged)
		{
			// TODO: 추후 MRT 지원 시 로직이 바뀌어야함
			pContext->OMSetBlendState(m_pBlendState, s_blendDesc.RTDesc[0].customFactor, s_blendDesc.RTDesc[0].writeMask);

			m_bBlendStateChaged = false;
		}
	}
}