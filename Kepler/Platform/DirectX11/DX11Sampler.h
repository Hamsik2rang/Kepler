#pragma once

#include <d3d11.h>

#include "Core/Renderer/Sampler.h"

namespace kepler {

class DX11Sampler : public ISampler
{
private:
	ID3D11SamplerState* m_pSamplerState;

public:
	DX11Sampler(ETexelComparer texelComparer,
		ESamplerFilter minFilter,
		ESamplerFilter magFilter,
		ESamplerFilter mipFilter,
		ESamplerAddress addressU,
		ESamplerAddress addressV,
		ESamplerAddress addressW,
		ESamplerComparer comparer,
		float minLOD,
		float maxLOD);

	~DX11Sampler();

	virtual void Bind(uint32_t slot) override;
	virtual void Unbind(uint32_t slot) override;
	virtual void SetSampler(const void* const pInSampler) override;
	virtual void GetSampler(void* pOutSampler) override;
};
}