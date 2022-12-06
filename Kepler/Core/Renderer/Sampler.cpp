#include "kepch.h"

#include "Sampler.h"
#include "GraphicsAPI.h"
#include "Platform/DirectX11/DX11Sampler.h"

namespace kepler {

std::shared_ptr<ISampler> ISampler::Create(
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
	EGraphicsAPI api = IGraphicsAPI::GetAPI();

	switch (api)
	{
	case EGraphicsAPI::None:
	{
		KEPLER_CORE_ASSERT(false, "None Graphics API Not Supported.");
		return nullptr;
	}
	break;
	case EGraphicsAPI::DirectX11:
	{
		return std::make_shared<DX11Sampler>(texelComparer, minFilter, magFilter, mipFilter, addressU, addressV, addressW, comparer, minLOD, maxLOD);
	}
	break;
	}

	KEPLER_CORE_ASSERT(false, "Fail to create Sampler");
	return nullptr;
}


}