#include "kepch.h"

#include "GraphicsAPI.h"
#include "Platform/DirectX11/DX11API.h"
#include "Platform/Vulkan/VulkanAPI.h"

namespace kepler {

eGraphicsAPI IGraphicsAPI::s_API = eGraphicsAPI::DirectX11;

IGraphicsAPI* IGraphicsAPI::Create()
{
	switch (s_API)
	{
	case eGraphicsAPI::None:
		KEPLER_CORE_ASSERT(false, "EGraphicsAPI::None is not supported");
		return nullptr;
	case eGraphicsAPI::DirectX11:
		return new DX11API;
	case eGraphicsAPI::Vulkan:
		return new VulkanAPI;
	}

	KEPLER_CORE_ASSERT(false, "unknown API");
	return nullptr;
}

}