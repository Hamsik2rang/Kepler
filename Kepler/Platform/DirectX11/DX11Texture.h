#pragma once

#include "DX11Texture.h"


namespace kepler {
	class DX11Texture
	{
	public:
		DX11Texture();
		DX11Texture(const DX11Texture& other);
		~DX11Texture();

		bool Init(ID3D11Device* pDevice, const WCHAR* pFilename);
		void Shutdown();

		inline ID3D11ShaderResourceView* GetTexture() { return m_pTexture; }


	private:
		ID3D11ShaderResourceView* m_pTexture = nullptr;
	};
}