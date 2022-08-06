#pragma once

#include <d3d11.h>

#include "Renderer/Sampler.h"

namespace kepler {

	class DX11Sampler : public ISampler
	{
	private:
		ID3D11SamplerState* m_pSamplerState;

	public:
		DX11Sampler(eTexelComparer texelComparer,
			eSamplerFilter minFilter,
			eSamplerFilter magFilter,
			eSamplerFilter mipFilter,
			eSamplerAddress addressU,
			eSamplerAddress addressV,
			eSamplerAddress addressW,
			eSamplerComparer comparer,
			float minLOD,
			float maxLOD);

		~DX11Sampler();

		virtual void Bind(uint32_t slot) override;
		virtual void Unbind(uint32_t slot) override;
		virtual void SetSampler(const void* const pInSampler) override;
		virtual void GetSampler(void* pOutSampler) override;
	};
}