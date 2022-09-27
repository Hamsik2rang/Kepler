#include "kepch.h"

#include "DX11Sampler.h"
#include "DX11Context.h"

namespace kepler {

	DX11Sampler::DX11Sampler(
		ETexelComparer texelComparer,
		ESamplerFilter minFilter,
		ESamplerFilter magFilter,
		ESamplerFilter mipFilter,
		ESamplerAddress addressU,
		ESamplerAddress addressV,
		ESamplerAddress addressW,
		ESamplerComparer comparer,
		float minLOD,
		float maxLOD)
	{
		// bit configuration of D3D11_FILTER enum type (16bit)
		//  15	 ...   10 9            8      7      6        5        4        3        2        1        0(LSB)
		//  +----------- + ----------- + ----------- + --------------  + --------------  + --------------- +
		//	|     0      |  0b00~0b11  |    0 or 1   |    0b00~0b11    |    0b00~0b11    |    0b00~0b11    |   
		//	+----------- + ----------- +------------ + --------------  + --------------  + --------------- +
		//	| not used   |   Comparer  | anisotropic |    min filter   |    mag filter   |    mip filter   |
		//	+---- 3 ---- + ----- 2 --- + ---- 1 ---- + ------ 2 ------ + ------ 2 ------ + ------ 2 ------ +

		D3D11_SAMPLER_DESC desc{};
		int filterFlags = 0;
		switch (texelComparer)
		{
		case ETexelComparer::None:			break;
		case ETexelComparer::Comparison:	filterFlags |= BIT(8); break;
		case ETexelComparer::Minimum:		filterFlags |= BIT(9); break;
		case ETexelComparer::Maximum:		filterFlags |= (BIT(8) | BIT(9)); break;
		}

		switch (minFilter)
		{
		case ESamplerFilter::None:
		case ESamplerFilter::Point:			break;
		case ESamplerFilter::Anisotropic:	filterFlags |= BIT(7); 
			[[fallthrough]];
		case ESamplerFilter::Linear:		filterFlags |= BIT(5); break;
		}

		switch (magFilter)
		{
		case ESamplerFilter::None:
		case ESamplerFilter::Point:			break;
		case ESamplerFilter::Anisotropic:	filterFlags |= BIT(7);
			[[fallthrough]];
		case ESamplerFilter::Linear:		filterFlags |= BIT(3); break;
		}

		switch (mipFilter)
		{
		case ESamplerFilter::None:
		case ESamplerFilter::Point:			break;
		case ESamplerFilter::Anisotropic:	filterFlags |= BIT(7);
			[[fallthrough]];
		case ESamplerFilter::Linear:		filterFlags |= BIT(1); break;
		}

		desc.Filter = (D3D11_FILTER)filterFlags;
		switch (comparer)
		{
		case ESamplerComparer::Never:			desc.ComparisonFunc = D3D11_COMPARISON_NEVER;			break;
		case ESamplerComparer::Less:			desc.ComparisonFunc = D3D11_COMPARISON_LESS;			break;
		case ESamplerComparer::Equal:			desc.ComparisonFunc = D3D11_COMPARISON_EQUAL;			break;
		case ESamplerComparer::LessOrEqual:		desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;		break;
		case ESamplerComparer::Greater:			desc.ComparisonFunc = D3D11_COMPARISON_GREATER;			break;
		case ESamplerComparer::GreaterOrEqual:	desc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;	break;
		case ESamplerComparer::Always:			desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;			break;
		}

		switch (addressU)
		{
		case ESamplerAddress::Wrap:			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; break;
		case ESamplerAddress::Mirror:		desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR; break;
		case ESamplerAddress::Clamp:		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP; break;
		case ESamplerAddress::Border:		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER; break;
		case ESamplerAddress::MirrorOnce:	desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE; break;
		}

		switch (addressV)
		{
		case ESamplerAddress::Wrap:			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; break;
		case ESamplerAddress::Mirror:		desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR; break;
		case ESamplerAddress::Clamp:		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP; break;
		case ESamplerAddress::Border:		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER; break;
		case ESamplerAddress::MirrorOnce:	desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE; break;
		}

		switch (addressW)
		{
		case ESamplerAddress::Wrap:			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; break;
		case ESamplerAddress::Mirror:		desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR; break;
		case ESamplerAddress::Clamp:		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP; break;
		case ESamplerAddress::Border:		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER; break;
		case ESamplerAddress::MirrorOnce:	desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE; break;
		}

		desc.MinLOD = minLOD;
		desc.MaxLOD = maxLOD;

		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		HRESULT hr = pDevice->CreateSamplerState(&desc, &m_pSamplerState);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to create DX11 Sampler State.");
		}
	}

	DX11Sampler::~DX11Sampler()
	{
		if (m_pSamplerState)
		{
			m_pSamplerState->Release();
			m_pSamplerState = nullptr;
		}
	}

	void DX11Sampler::Bind(uint32_t slot)
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		pContext->PSSetSamplers(slot, 1, &m_pSamplerState);
	}

	void DX11Sampler::Unbind(uint32_t slot)
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();
		pContext->PSSetSamplers(slot, 1, nullptr);
	}

	void DX11Sampler::SetSampler(const void* const pInSampler)
	{
		if (m_pSamplerState)
		{
			m_pSamplerState->Release();
			m_pSamplerState = nullptr;
		}
		m_pSamplerState = (ID3D11SamplerState*)pInSampler;
	}

	void DX11Sampler::GetSampler(void* pOutSampler)
	{
		pOutSampler = m_pSamplerState;
	}
}