#pragma once

#include "Core/Base.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/Texture.h"

namespace kepler {

	class DX11FrameBuffer : public IFrameBuffer
	{
	private:
		static constexpr uint8_t s_maxGBufferCount = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;

		// G-Buffers. [0] is Color Buffer(SV_TARGET0)
		ID3D11RenderTargetView*		m_pRenderTargetViews[s_maxGBufferCount];
		std::shared_ptr<ITexture2D> m_pTextures[s_maxGBufferCount];
		ID3D11DepthStencilView*		m_pDepthStencilView;

	public:
		DX11FrameBuffer();
		~DX11FrameBuffer();

		virtual void Init() override;

		virtual void ClearColor(const float color[4]) override;
		virtual void ClearDepthStencil(bool bDepthClear, bool bStencilClear, const float depth, const uint8_t stencil) override;
		virtual void ClearGBuffer(const float color[4]) override;
		virtual void ClearGBuffer(uint8_t index, const float color[4]) override;
		virtual void ClearGBuffer(uint8_t startSlot, uint8_t count, const float color[4]) override;

		virtual void BindColorBuffer() override;
		virtual void UnbindColorBuffer() override;

		virtual void BindGBuffer(uint8_t startSlot, uint8_t count) override;
		virtual void UnbindGBuffer(uint8_t startSlot, uint8_t count) override;

		virtual void AddGBuffer(uint8_t startSlot, uint8_t count) override;
		virtual void DeleteGBuffer(uint8_t startSlot, uint8_t count) override;

		virtual void* GetBuffer(eFrameBufferType type, uint8_t index) override;
	};
}