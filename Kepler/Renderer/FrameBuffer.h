#pragma once

#include "Core/Base.h"
#include "KeplerMath.h"

namespace kepler {

	enum class eFrameBufferType
	{
		R32G32B32A32 = 0,
		Uint8Unorm24,

		Color = R32G32B32A32,
		DepthStencil = Uint8Unorm24
	};

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

		virtual void ResizeGBuffer(uint8_t startSlot, uint8_t count, uint32_t width, uint32_t height) = 0;
		virtual void ResizeColorBuffer(uint32_t width, uint32_t height) = 0;

		virtual void* GetBuffer(eFrameBufferType type, uint8_t index) = 0;

		static std::shared_ptr<IFrameBuffer> Create();
		static std::shared_ptr<IFrameBuffer> Get();
	};
}