#pragma once

#include "Core/Base.h"
#include "RenderState.h"

namespace kepler {

	enum class ETexelComparer
	{
		None = 0,
		Comparison,
		Minimum,
		Maximum
	};

	enum class ESamplerFilter
	{
		None = 0,
		Point,
		Linear,
		Anisotropic,
	};

	enum class ESamplerAddress
	{
		Wrap = 1,
		Mirror,
		Clamp,
		Border,
		MirrorOnce,
	};

	using ESamplerComparer = EFrameBufferComparer;

	class ISampler
	{
	public:
		virtual ~ISampler() = default;
		virtual void Bind(uint32_t slot) = 0;
		virtual void Unbind(uint32_t slot) = 0;

		virtual void SetSampler(const void* const pInSampler) = 0;
		virtual void GetSampler(void* pOutSampler) = 0;

		static std::shared_ptr<ISampler> Create(
			ETexelComparer texelComparer,
			ESamplerFilter minFilter,
			ESamplerFilter magFilter,
			ESamplerFilter mipFilter,
			ESamplerAddress addressU,
			ESamplerAddress addressV,
			ESamplerAddress addressW,
			ESamplerComparer comparer,
			float minLOD,
			float maxLOD
		);
	};
}