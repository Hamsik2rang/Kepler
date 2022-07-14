#pragma once

#include "Core/Base.h"
#include "KeplerMath.h"

namespace kepler {

	class IFrameBuffer
	{
	private:
		static std::shared_ptr<IFrameBuffer> s_pInstance;

	public:
		virtual ~IFrameBuffer() = default;

		virtual void Init() = 0;

		virtual void ClearColor(const float color[4]) = 0;
		virtual void ClearGBuffer(const float color[4]) = 0;
		virtual void ClearGBuffer(uint8_t index, const float color[4]) = 0;
		virtual void ClearGBuffer(uint8_t startSlot, uint8_t count, const float color[4]) = 0;
		virtual void ClearDepthStencil(bool bDepthClear, bool bStencilClear, const float depth, const uint8_t stencil) = 0;

		virtual void BindColorBuffer() = 0;
		virtual void UnbindColorBuffer() = 0;
		
		virtual void BindGBuffer(uint8_t startSlot, uint8_t count) = 0;
		virtual void UnbindGBuffer(uint8_t startSlot, uint8_t count) = 0;

		virtual void AddGBuffer(uint8_t startSlot, uint8_t count) = 0;
		virtual void DeleteGBuffer(uint8_t startSlot, uint8_t count) = 0;

		static std::shared_ptr<IFrameBuffer> Create();
		static std::shared_ptr<IFrameBuffer> Get();
	};
}