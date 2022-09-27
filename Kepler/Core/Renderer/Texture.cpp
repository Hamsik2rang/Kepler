#include "kepch.h"

#include "Texture.h"
#include "Core/Renderer/GraphicsAPI.h"
#include "Platform/DirectX11/DX11Texture.h"

namespace kepler {

	std::shared_ptr<ITexture1D> ITexture1D::Create(const ETextureDataType type, const uint32_t length, const uint8_t channel, const uint8_t bytePerTexel)
	{
		EGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case EGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not supported");
				return nullptr;
			}
		case EGraphicsAPI::DirectX11:
			{
				//return std::make_shared<DX11Texture1D>(type, length, channel, bytePerTexel);
				return nullptr;
			}
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

	std::shared_ptr<ITexture2D> ITexture2D::Create(const ETextureDataType type, 
												   const uint32_t width, 
												   const uint32_t height, 
												   const uint8_t channel, 
												   const uint8_t bytePerTexel)
	{
		EGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case EGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not supported");
				return nullptr;
			}
		case EGraphicsAPI::DirectX11:
			{
				return std::make_shared<DX11Texture2D>(type, width, height, channel, bytePerTexel);
			}
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

	std::shared_ptr<ITexture2D> ITexture2D::Create(const ETextureDataType type, const std::string& filepath)
	{
		EGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case EGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not supported");
				return nullptr;
			}
		case EGraphicsAPI::DirectX11:
			{
				return std::make_shared<DX11Texture2D>(type, filepath);
			}
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

	std::shared_ptr<ITexture3D> ITexture3D::Create(const ETextureDataType type,
												   const uint32_t width,
												   const uint32_t height,
												   const uint32_t depth,
												   const uint8_t channel, 
												   const uint8_t bytePerTexel)
	{
		EGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case EGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not supported");
				return nullptr;
			}
		case EGraphicsAPI::DirectX11:
			{
				//return std::make_shared<DX11Texture3D>(type, width, height, depth, channel, bytePerTexel);
				return nullptr;
			}
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

}