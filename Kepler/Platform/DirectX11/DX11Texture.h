#pragma once

#include "Renderer/Texture.h"
#include "DX11Texture.h"
#include "Renderer/Shader.h"

namespace kepler {
	// Texture1D, Texture3D는 나중에 필요할 때 구현합시다.
	class DX11Texture2D : public ITexture2D
	{
	private:
		ID3D11Texture2D* m_pTexture;
		ID3D11ShaderResourceView* m_pResourceView;

		uint32_t m_width;
		uint32_t m_height;

	public:
		DX11Texture2D(const eTextureDataType type, const uint32_t width, const uint32_t height, const uint8_t channel = 0, const uint8_t bytePerTexel = 4);
		DX11Texture2D(const eTextureDataType type, const std::string& filepath);

		~DX11Texture2D();

		// Inherited via ITexture2D
		virtual void Bind(const uint32_t slot) override;
		virtual void SetData(const void* pData, const uint32_t size) override;
		virtual void* GetData() override { return m_pResourceView; }
		virtual void* GetTexture() override { return m_pTexture; }
		inline virtual uint32_t GetWidth() const override { return m_width; }
		inline virtual uint32_t GetHeight() const override { return m_height; }
	};
}