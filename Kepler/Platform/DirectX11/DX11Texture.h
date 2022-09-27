#pragma once

#include "Core/Renderer/Texture.h"
#include "DX11Texture.h"
#include "Core/Renderer/Shader.h"

namespace kepler {
	// Texture1D, Texture3D�� ���߿� �ʿ��� �� �����սô�.
	class DX11Texture2D : public ITexture2D
	{
	private:
		ID3D11Texture2D* m_pTexture;
		ID3D11ShaderResourceView* m_pResourceView;

		ETextureDataType m_type;

		uint32_t m_width;
		uint32_t m_height;

	public:
		DX11Texture2D(const ETextureDataType type, const uint32_t width, const uint32_t height, const uint8_t channel = 0, const uint8_t bytePerTexel = 4);
		DX11Texture2D(const ETextureDataType type, const std::string& filepath);

		~DX11Texture2D();

		// Inherited via ITexture2D
		virtual void Bind(const uint32_t slot) override;
		virtual void SetData(const void* pData, const uint32_t width, const uint32_t height) override;
		inline virtual void* GetData() override { return m_pResourceView; }
		inline virtual void* GetTexture() override { return m_pTexture; }
		inline virtual ETextureDataType GetType() override { return m_type; }
		inline virtual uint32_t GetWidth() const override { return m_width; }
		inline virtual uint32_t GetHeight() const override { return m_height; }
	};
}