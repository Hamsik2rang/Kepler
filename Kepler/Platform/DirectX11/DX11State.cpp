#include "kepch.h"

#include "DX11State.h"
#include "DX11FrameBuffer.h"
#include "Core/Renderer/GraphicsContext.h"

namespace kepler {

	DX11State::DX11State()
		: m_pBlendState{ nullptr }
		, m_pDepthStencilState{ nullptr }
		, m_bShaderStateChaged{ false }
		, m_pRasterizerState{ nullptr }
		, m_bBlendStateChaged{ false }
		, m_bRasterizerStateChanged{ false }
		, m_bDepthStateChanged{ false }
		, m_bStencilStateChaged{ false }
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

		m_bShaderStateChaged = true;
	}

	void DX11State::CreateRasterizerState()
	{
		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		// �̹� ���������ο� �� �ִ� ���� State ����
		if (m_pRasterizerState)
		{
			pContext->RSSetState(nullptr);
			m_pRasterizerState->Release();
			m_pRasterizerState = nullptr;
		}

		D3D11_RASTERIZER_DESC desc{};

		switch (s_rasterizerDesc.cullMode)
		{
		case eCullMode::Off:	desc.CullMode = D3D11_CULL_NONE;	break;
		case eCullMode::Front:	desc.CullMode = D3D11_CULL_FRONT;	break;
		case eCullMode::Back:	desc.CullMode = D3D11_CULL_BACK;	break;
		}

		if (s_rasterizerDesc.bWireFrame)	desc.FillMode = D3D11_FILL_WIREFRAME;
		else								desc.FillMode = D3D11_FILL_SOLID;

		desc.FrontCounterClockwise = !s_rasterizerDesc.bIsFrontClockwise;
		desc.DepthBias = s_rasterizerDesc.depthBias;
		desc.SlopeScaledDepthBias = s_rasterizerDesc.depthSlopedBias;
		desc.DepthBiasClamp = s_rasterizerDesc.depthClampBias;
		desc.DepthClipEnable = s_rasterizerDesc.bDepthClip;
		desc.ScissorEnable = s_rasterizerDesc.bScissssor;
		desc.AntialiasedLineEnable = s_rasterizerDesc.bAntiAliasedLine;
		desc.MultisampleEnable = s_rasterizerDesc.bMultiSampling;

		HRESULT hr = pDevice->CreateRasterizerState(&desc, &m_pRasterizerState);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to crate Rasterizer State");
			return;
		}
	}

	void DX11State::CreateDepthStencilState()
	{
		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		// �̹� ���������ο� �� �ִ� ���� State ����
		if (m_pDepthStencilState)
		{
			pContext->OMSetDepthStencilState(nullptr, 0x00);
			m_pDepthStencilState->Release();
			m_pDepthStencilState = nullptr;
		}

		D3D11_DEPTH_STENCIL_DESC desc{};
		// ���� description�� �����Ѵٸ� ���������ο��� ������
		if (m_pDepthStencilState)
		{
			m_pDepthStencilState->GetDesc(&desc);
		}

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
				KEPLER_CORE_ASSERT(static_cast<bool>(desc.DepthEnable) == false, "None Depth Comparer is not supported when Depth test is enabled");
			}
			break;
		}

		if (s_stencilDesc.bStencilTest) desc.StencilEnable = true;
		else							desc.StencilEnable = false;

		desc.StencilReadMask = s_stencilDesc.readMask;
		desc.StencilWriteMask = s_stencilDesc.writeMask;

		pDevice->CreateDepthStencilState(&desc, &m_pDepthStencilState);
	}

	void DX11State::CreateBlendState()
	{
		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		// �̹� ���������ο� �� �ִ� ���� State ����
		if (m_pBlendState)
		{
			pContext->OMSetBlendState(nullptr, nullptr, 0x00);
			m_pBlendState->Release();
			m_pBlendState = nullptr;
		}
		D3D11_BLEND_DESC desc{};
		desc.AlphaToCoverageEnable = s_blendDesc.bUseAlphaToCoverage;
		desc.IndependentBlendEnable = s_blendDesc.bIndependentBlend;
		for (int i = 0; i < (desc.IndependentBlendEnable ? 1 : 8); i++)
		{
			D3D11_RENDER_TARGET_BLEND_DESC& rtDesc = desc.RenderTarget[i];
			rtDesc.BlendEnable = s_blendDesc.renderTarget[i].bBlendEnable;
			rtDesc.RenderTargetWriteMask = s_blendDesc.renderTarget[i].writeMask;

			switch (s_blendDesc.renderTarget[i].colorBlendOperator)
			{
			case eBlendOperator::Add:		rtDesc.BlendOp = D3D11_BLEND_OP_ADD;			break;
			case eBlendOperator::Sub:		rtDesc.BlendOp = D3D11_BLEND_OP_SUBTRACT;		break;
			case eBlendOperator::RevSub:	rtDesc.BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;	break;
			case eBlendOperator::Max:		rtDesc.BlendOp = D3D11_BLEND_OP_MAX;			break;
			case eBlendOperator::Min:		rtDesc.BlendOp = D3D11_BLEND_OP_MIN;			break;
			}

			switch (s_blendDesc.renderTarget[i].alphaBlendOperator)
			{
			case eBlendOperator::Add:		rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;			break;
			case eBlendOperator::Sub:		rtDesc.BlendOpAlpha = D3D11_BLEND_OP_SUBTRACT;		break;
			case eBlendOperator::RevSub:	rtDesc.BlendOpAlpha = D3D11_BLEND_OP_REV_SUBTRACT;	break;
			case eBlendOperator::Max:		rtDesc.BlendOpAlpha = D3D11_BLEND_OP_MAX;			break;
			case eBlendOperator::Min:		rtDesc.BlendOpAlpha = D3D11_BLEND_OP_MIN;			break;
			}

			switch (s_blendDesc.renderTarget[i].srcColorFactor)
			{
			case eBlendFactor::Zero:			rtDesc.SrcBlend = D3D11_BLEND_ZERO;				break;
			case eBlendFactor::One:				rtDesc.SrcBlend = D3D11_BLEND_ONE;				break;
			case eBlendFactor::SrcColor:		rtDesc.SrcBlend = D3D11_BLEND_SRC_COLOR;		break;
			case eBlendFactor::InvSrcColor:		rtDesc.SrcBlend = D3D11_BLEND_INV_SRC_COLOR;	break;
			case eBlendFactor::SrcAlpha:		rtDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;		break;
			case eBlendFactor::InvSrcAlpha:		rtDesc.SrcBlend = D3D11_BLEND_INV_SRC_ALPHA;	break;
			case eBlendFactor::DestAlpha:		rtDesc.SrcBlend = D3D11_BLEND_DEST_ALPHA;		break;
			case eBlendFactor::InvDestAlpha:	rtDesc.SrcBlend = D3D11_BLEND_INV_DEST_ALPHA;	break;
			case eBlendFactor::DestColor:		rtDesc.SrcBlend = D3D11_BLEND_DEST_COLOR;		break;
			case eBlendFactor::InvDestColor:	rtDesc.SrcBlend = D3D11_BLEND_INV_DEST_COLOR;	break;
			case eBlendFactor::SrcAlphaClamp:	rtDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA_SAT;	break;
			case eBlendFactor::CustomFactor:	rtDesc.SrcBlend = D3D11_BLEND_BLEND_FACTOR;		break;
			case eBlendFactor::InvCustomFactor: rtDesc.SrcBlend = D3D11_BLEND_INV_BLEND_FACTOR;	break;
			case eBlendFactor::DualSrcColor:	rtDesc.SrcBlend = D3D11_BLEND_SRC1_COLOR;		break;
			case eBlendFactor::DualInvSrcColor: rtDesc.SrcBlend = D3D11_BLEND_INV_SRC1_COLOR;	break;
			case eBlendFactor::DualSrcAlpha:	rtDesc.SrcBlend = D3D11_BLEND_SRC1_ALPHA;		break;
			case eBlendFactor::DualInvSrcAlpha: rtDesc.SrcBlend = D3D11_BLEND_INV_SRC1_ALPHA;	break;
			}

			switch (s_blendDesc.renderTarget[i].srcAlphaFactor)
			{
			case eBlendFactor::Zero:			rtDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;				break;
			case eBlendFactor::One:				rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;					break;
			case eBlendFactor::SrcColor:		rtDesc.SrcBlendAlpha = D3D11_BLEND_SRC_COLOR;			break;
			case eBlendFactor::InvSrcColor:		rtDesc.SrcBlendAlpha = D3D11_BLEND_INV_SRC_COLOR;		break;
			case eBlendFactor::SrcAlpha:		rtDesc.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;			break;
			case eBlendFactor::InvSrcAlpha:		rtDesc.SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;		break;
			case eBlendFactor::DestAlpha:		rtDesc.SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;			break;
			case eBlendFactor::InvDestAlpha:	rtDesc.SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;		break;
			case eBlendFactor::DestColor:		rtDesc.SrcBlendAlpha = D3D11_BLEND_DEST_COLOR;			break;
			case eBlendFactor::InvDestColor:	rtDesc.SrcBlendAlpha = D3D11_BLEND_INV_DEST_COLOR;		break;
			case eBlendFactor::SrcAlphaClamp:	rtDesc.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA_SAT;		break;
			case eBlendFactor::CustomFactor:	rtDesc.SrcBlendAlpha = D3D11_BLEND_BLEND_FACTOR;		break;
			case eBlendFactor::InvCustomFactor: rtDesc.SrcBlendAlpha = D3D11_BLEND_INV_BLEND_FACTOR;	break;
			case eBlendFactor::DualSrcColor:	rtDesc.SrcBlendAlpha = D3D11_BLEND_SRC1_COLOR;			break;
			case eBlendFactor::DualInvSrcColor: rtDesc.SrcBlendAlpha = D3D11_BLEND_INV_SRC1_COLOR;		break;
			case eBlendFactor::DualSrcAlpha:	rtDesc.SrcBlendAlpha = D3D11_BLEND_SRC1_ALPHA;			break;
			case eBlendFactor::DualInvSrcAlpha: rtDesc.SrcBlendAlpha = D3D11_BLEND_INV_SRC1_ALPHA;		break;
			}

			switch (s_blendDesc.renderTarget[i].destColorFactor)
			{
			case eBlendFactor::Zero:			rtDesc.DestBlend = D3D11_BLEND_ZERO;				break;
			case eBlendFactor::One:				rtDesc.DestBlend = D3D11_BLEND_ONE;					break;
			case eBlendFactor::SrcColor:		rtDesc.DestBlend = D3D11_BLEND_SRC_COLOR;			break;
			case eBlendFactor::InvSrcColor:		rtDesc.DestBlend = D3D11_BLEND_INV_SRC_COLOR;		break;
			case eBlendFactor::SrcAlpha:		rtDesc.DestBlend = D3D11_BLEND_SRC_ALPHA;			break;
			case eBlendFactor::InvSrcAlpha:		rtDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		break;
			case eBlendFactor::DestAlpha:		rtDesc.DestBlend = D3D11_BLEND_DEST_ALPHA;			break;
			case eBlendFactor::InvDestAlpha:	rtDesc.DestBlend = D3D11_BLEND_INV_DEST_ALPHA;		break;
			case eBlendFactor::DestColor:		rtDesc.DestBlend = D3D11_BLEND_DEST_COLOR;			break;
			case eBlendFactor::InvDestColor:	rtDesc.DestBlend = D3D11_BLEND_INV_DEST_COLOR;		break;
			case eBlendFactor::SrcAlphaClamp:	rtDesc.DestBlend = D3D11_BLEND_SRC_ALPHA_SAT;		break;
			case eBlendFactor::CustomFactor:	rtDesc.DestBlend = D3D11_BLEND_BLEND_FACTOR;		break;
			case eBlendFactor::InvCustomFactor: rtDesc.DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;	break;
			case eBlendFactor::DualSrcColor:	rtDesc.DestBlend = D3D11_BLEND_SRC1_COLOR;			break;
			case eBlendFactor::DualInvSrcColor: rtDesc.DestBlend = D3D11_BLEND_INV_SRC1_COLOR;		break;
			case eBlendFactor::DualSrcAlpha:	rtDesc.DestBlend = D3D11_BLEND_SRC1_ALPHA;			break;
			case eBlendFactor::DualInvSrcAlpha: rtDesc.DestBlend = D3D11_BLEND_INV_SRC1_ALPHA;		break;
			}

			switch (s_blendDesc.renderTarget[i].destAlphaFactor)
			{
			case eBlendFactor::Zero:			rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;				break;
			case eBlendFactor::One:				rtDesc.DestBlendAlpha = D3D11_BLEND_ONE;				break;
			case eBlendFactor::SrcColor:		rtDesc.DestBlendAlpha = D3D11_BLEND_SRC_COLOR;			break;
			case eBlendFactor::InvSrcColor:		rtDesc.DestBlendAlpha = D3D11_BLEND_INV_SRC_COLOR;		break;
			case eBlendFactor::SrcAlpha:		rtDesc.DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;			break;
			case eBlendFactor::InvSrcAlpha:		rtDesc.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;		break;
			case eBlendFactor::DestAlpha:		rtDesc.DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;			break;
			case eBlendFactor::InvDestAlpha:	rtDesc.DestBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;		break;
			case eBlendFactor::DestColor:		rtDesc.DestBlendAlpha = D3D11_BLEND_DEST_COLOR;			break;
			case eBlendFactor::InvDestColor:	rtDesc.DestBlendAlpha = D3D11_BLEND_INV_DEST_COLOR;		break;
			case eBlendFactor::SrcAlphaClamp:	rtDesc.DestBlendAlpha = D3D11_BLEND_SRC_ALPHA_SAT;		break;
			case eBlendFactor::CustomFactor:	rtDesc.DestBlendAlpha = D3D11_BLEND_BLEND_FACTOR;		break;
			case eBlendFactor::InvCustomFactor: rtDesc.DestBlendAlpha = D3D11_BLEND_INV_BLEND_FACTOR;	break;
			case eBlendFactor::DualSrcColor:	rtDesc.DestBlendAlpha = D3D11_BLEND_SRC1_COLOR;			break;
			case eBlendFactor::DualInvSrcColor: rtDesc.DestBlendAlpha = D3D11_BLEND_INV_SRC1_COLOR;		break;
			case eBlendFactor::DualSrcAlpha:	rtDesc.DestBlendAlpha = D3D11_BLEND_SRC1_ALPHA;			break;
			case eBlendFactor::DualInvSrcAlpha: rtDesc.DestBlendAlpha = D3D11_BLEND_INV_SRC1_ALPHA;		break;
			}
		}

		pDevice->CreateBlendState(&desc, &m_pBlendState);
	}

	void DX11State::Bind()
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		if (m_bShaderStateChaged)
		{
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
			pContext->OMSetDepthStencilState(m_pDepthStencilState, s_stencilDesc.stencilRef);

			m_bDepthStateChanged = false;
			m_bStencilStateChaged = false;
		}
		if (m_bBlendStateChaged)
		{
			CreateBlendState();
			// TODO: ���� MRT ���� �� ������ �ٲ�����
			pContext->OMSetBlendState(m_pBlendState, s_blendDesc.customFactor, s_blendDesc.sampleMask);

			m_bBlendStateChaged = false;
		}
	}
}
