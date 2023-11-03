#include "kepch.h"

#include "DX11Sampler.h"
#include "DX11Context.h"

namespace kepler {

DX11Sampler::DX11Sampler(
	eTexelComparer texelComparer,
	eSamplerFilter minFilter,
	eSamplerFilter magFilter,
	eSamplerFilter mipFilter,
	eSamplerAddress addressU,
	eSamplerAddress addressV,
	eSamplerAddress addressW,
	eSamplerComparer comparer,
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
	case eTexelComparer::None:			break;
	case eTexelComparer::Comparison:	filterFlags |= BIT(8); break;
	case eTexelComparer::Minimum:		filterFlags |= BIT(9); break;
	case eTexelComparer::Maximum:		filterFlags |= (BIT(8) | BIT(9)); break;
	}

	switch (minFilter)
	{
	case eSamplerFilter::None:
	case eSamplerFilter::Point:			break;
	case eSamplerFilter::Anisotropic:	filterFlags |= BIT(6);
		[[fallthrough]];
	case eSamplerFilter::Linear:		filterFlags |= BIT(4); break;
	}

	switch (magFilter)
	{
	case eSamplerFilter::None:
	case eSamplerFilter::Point:			break;
	case eSamplerFilter::Anisotropic:	filterFlags |= BIT(6);
		[[fallthrough]];
	case eSamplerFilter::Linear:		filterFlags |= BIT(2); break;
	}

	switch (mipFilter)
	{
	case eSamplerFilter::None:
	case eSamplerFilter::Point:			break;
	case eSamplerFilter::Anisotropic:	filterFlags |= BIT(6);
		[[fallthrough]];
	case eSamplerFilter::Linear:		filterFlags |= BIT(0); break;
	}

	desc.Filter = (D3D11_FILTER)filterFlags;
	switch (comparer)
	{
	case eSamplerComparer::Never:			desc.ComparisonFunc = D3D11_COMPARISON_NEVER;			break;
	case eSamplerComparer::Less:			desc.ComparisonFunc = D3D11_COMPARISON_LESS;			break;
	case eSamplerComparer::Equal:			desc.ComparisonFunc = D3D11_COMPARISON_EQUAL;			break;
	case eSamplerComparer::LessOrEqual:		desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;		break;
	case eSamplerComparer::Greater:			desc.ComparisonFunc = D3D11_COMPARISON_GREATER;			break;
	case eSamplerComparer::GreaterOrEqual:	desc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;	break;
	case eSamplerComparer::Always:			desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;			break;
	}

	switch (addressU)
	{
	case eSamplerAddress::Wrap:			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; break;
	case eSamplerAddress::Mirror:		desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR; break;
	case eSamplerAddress::Clamp:		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP; break;
	case eSamplerAddress::Border:		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER; break;
	case eSamplerAddress::MirrorOnce:	desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE; break;
	}

	switch (addressV)
	{
	case eSamplerAddress::Wrap:			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; break;
	case eSamplerAddress::Mirror:		desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR; break;
	case eSamplerAddress::Clamp:		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP; break;
	case eSamplerAddress::Border:		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER; break;
	case eSamplerAddress::MirrorOnce:	desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE; break;
	}

	switch (addressW)
	{
	case eSamplerAddress::Wrap:			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; break;
	case eSamplerAddress::Mirror:		desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR; break;
	case eSamplerAddress::Clamp:		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP; break;
	case eSamplerAddress::Border:		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER; break;
	case eSamplerAddress::MirrorOnce:	desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE; break;
	}

	desc.MinLOD = minLOD;
	desc.MaxLOD = maxLOD;

	ID3D11Device* pDevice = static_cast<ID3D11Device*>(IGraphicsContext::Get()->GetDevice());
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
	ID3D11DeviceContext* pContext = static_cast<ID3D11DeviceContext*>(IGraphicsContext::Get()->GetDeviceContext());
	pContext->PSSetSamplers(slot, 1, &m_pSamplerState);
}

void DX11Sampler::Unbind(uint32_t slot)
{
	ID3D11DeviceContext* pContext = static_cast<ID3D11DeviceContext*>(IGraphicsContext::Get()->GetDeviceContext());
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