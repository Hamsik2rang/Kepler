#pragma once

#include "Renderer/Texture.h"
#include "DX11Texture.h"
#include "Renderer/Shader.h"

namespace kepler {
	// Texture1D, Texture3D는 나중에 필요할 때 구현합시다.
	class DX11Texture2D : public ITexture2D
	{
	private:
		ID3D11Texture2D*			m_pTexture;
		ID3D11ShaderResourceView*	m_pResourceView;
		ID3D11SamplerState*			m_pSamplerState;

		uint32_t m_width;
		uint32_t m_height;

	public:
		DX11Texture2D(const eTextureDataType type, const uint32_t width, const uint32_t height, const uint8_t channel, const uint8_t bytePerTexel);
		DX11Texture2D(const eTextureDataType type, const std::string& filepath);
		
		~DX11Texture2D();

		// Inherited via ITexture2D
		virtual void Bind(const uint32_t slot) override;
		virtual void SetData(const void* data, const uint32_t size) override;
		inline virtual uint32_t GetWidth() const override { return m_width; }
		inline virtual uint32_t GetHeight() const override { return m_height; }
	};
}