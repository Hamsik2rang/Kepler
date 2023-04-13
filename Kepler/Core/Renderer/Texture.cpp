#include "kepch.h"

#include "Texture.h"
#include "Core/Renderer/GraphicsAPI.h"
#include "Platform/DirectX11/DX11Texture.h"

namespace kepler {

std::shared_ptr<ITexture1D> ITexture1D::Create(const eTextureDataType type, const uint32_t length)
{
	eGraphicsAPI api = IGraphicsAPI::GetAPI();
	switch (api)
	{
	case eGraphicsAPI::None:
	{
		KEPLER_ASSERT(false, "None Graphics API is not supported");
		return nullptr;
	}
	case eGraphicsAPI::DirectX11:
	{
		//return std::make_shared<DX11Texture1D>(type, length);
		return nullptr;
	}
	}

	KEPLER_ASSERT(false, "Invalid Graphics API");
	return nullptr;
}

std::shared_ptr<ITexture2D> ITexture2D::Create(const eTextureDataType type,
	const uint32_t width,
	const uint32_t height)
{
	eGraphicsAPI api = IGraphicsAPI::GetAPI();
	switch (api)
	{
	case eGraphicsAPI::None:
	{
		KEPLER_ASSERT(false, "None Graphics API is not supported");
		return nullptr;
	}
	case eGraphicsAPI::DirectX11:
	{
		return std::make_shared<DX11Texture2D>(type, width, height);
	}
	}

	KEPLER_ASSERT(false, "Invalid Graphics API");
	return nullptr;
}

std::shared_ptr<ITexture2D> ITexture2D::Create(const eTextureDataType type, const std::string& filepath)
{
	eGraphicsAPI api = IGraphicsAPI::GetAPI();
	switch (api)
	{
	case eGraphicsAPI::None:
	{
		KEPLER_ASSERT(false, "None Graphics API is not supported");
		return nullptr;
	}
	case eGraphicsAPI::DirectX11:
	{
		return std::make_shared<DX11Texture2D>(type, filepath);
	}
	}

	KEPLER_ASSERT(false, "Invalid Graphics API");
	return nullptr;
}

std::shared_ptr<ITexture3D> ITexture3D::Create(const eTextureDataType type,
	const uint32_t width,
	const uint32_t height,
	const uint32_t depth)
{
	eGraphicsAPI api = IGraphicsAPI::GetAPI();
	switch (api)
	{
	case eGraphicsAPI::None:
	{
		KEPLER_ASSERT(false, "None Graphics API is not supported");
		return nullptr;
	}
	case eGraphicsAPI::DirectX11:
	{
		//return std::make_shared<DX11Texture3D>(type, width, height, depth);
		return nullptr;
	}
	}

	KEPLER_ASSERT(false, "Invalid Graphics API");
	return nullptr;
}

}