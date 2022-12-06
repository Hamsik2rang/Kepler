#pragma once

#include "Core/Renderer/Texture.h"
#include "DX11Texture.h"
#include "Core/Renderer/Shader.h"

namespace kepler {
// Texture1D, Texture3D는 나중에 필요할 때 구현합시다.
class DX11Texture2D : public ITexture2D
{
private:
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pResourceView;

	ETextureDataType m_type;

	uint32_t m_width;
	uint32_t m_height;
	uint8_t m_channel;

public:
	DX11Texture2D(const ETextureDataType type, const uint32_t width, const uint32_t height);
	DX11Texture2D(const ETextureDataType type, const std::string& filepath);

	~DX11Texture2D();

	// Inherited via ITexture2D
	virtual void Bind(const uint32_t slot) override;
	virtual void SetData(const void* pData, const uint32_t width, const uint32_t height, const uint32_t channel) override;
	inline virtual void* GetData() override { return m_pResourceView; }
	inline virtual void* GetTexture() override { return m_pTexture; }
	inline virtual ETextureDataType GetType() override { return m_type; }
	inline virtual uint32_t GetWidth() const override { return m_width; }
	inline virtual uint32_t GetHeight() const override { return m_height; }
};
}