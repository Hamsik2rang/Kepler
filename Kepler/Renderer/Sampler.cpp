#include "kepch.h"

#include "Sampler.h"
#include "GraphicsAPI.h"
#include "Platform/DirectX11/DX11Sampler.h"

namespace kepler {

std::shared_ptr<ISampler> ISampler::Create(
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
	eGraphicsAPI api = IGraphicsAPI::GetAPI();

	switch (api)
	{
	case eGraphicsAPI::None:
	{
		KEPLER_CORE_ASSERT(false, "None Graphics API Not Supported.");
		return nullptr;
	}
	break;
	case eGraphicsAPI::DirectX11:
	{
		return std::make_shared<DX11Sampler>(texelComparer, minFilter, magFilter, mipFilter, addressU, addressV, addressW, comparer, minLOD, maxLOD);
	}
	break;
	}

	KEPLER_CORE_ASSERT(false, "Fail to create Sampler");
	return nullptr;
}


}