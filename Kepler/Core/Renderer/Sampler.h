#pragma once

#include "Core/Base.h"
#include "RenderState.h"

namespace kepler {

	enum class eTexelComparer
	{
		None = 0,
		Comparison,
		Minimum,
		Maximum
	};

	enum class eSamplerFilter
	{
		None = 0,
		Point,
		Linear,
		Anisotropic,
	};

	enum class eSamplerAddress
	{
		Wrap = 1,
		Mirror,
		Clamp,
		Border,
		MirrorOnce,
	};

	using eSamplerComparer = eFrameBufferComparer;

	class ISampler
	{
	public:
		virtual ~ISampler() = default;
		virtual void Bind(uint32_t slot) = 0;
		virtual void Unbind(uint32_t slot) = 0;

		virtual void SetSampler(const void* const pInSampler) = 0;
		virtual void GetSampler(void* pOutSampler) = 0;

		static std::shared_ptr<ISampler> Create(
			eTexelComparer texelComparer,
			eSamplerFilter minFilter,
			eSamplerFilter magFilter,
			eSamplerFilter mipFilter,
			eSamplerAddress addressU,
			eSamplerAddress addressV,
			eSamplerAddress addressW,
			eSamplerComparer comparer,
			float minLOD,
			float maxLOD
		);
	};
}