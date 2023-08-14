#include "kepch.h"
#include "GraphicsContext.h"
#include "Platform/DirectX11/DX11Context.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace kepler {

IGraphicsContext* IGraphicsContext::s_pInstance = nullptr;

IGraphicsContext* IGraphicsContext::Create(const void* hWindow)
{
	if (s_pInstance)
	{
		return s_pInstance;
	}
	eGraphicsAPI api = IGraphicsAPI::GetAPI();

	switch (api)
	{
	case eGraphicsAPI::None:
	{
		KEPLER_CORE_ASSERT(false, "None API not supported");
	}
	break;
	case eGraphicsAPI::DirectX11:
	{
		const HWND* phWnd = static_cast<const HWND*>(hWindow);
		s_pInstance = new DX11Context(*phWnd);
	}
	break;
	case eGraphicsAPI::Vulkan:
	{
		const HWND* phWnd = static_cast<const HWND*>(hWindow);
		s_pInstance = new VulkanContext(*phWnd);
	}
	 break;
	default:
	{
		KEPLER_CORE_ASSERT(false, "Invalid Graphics API");
	}
	}

	return s_pInstance;
}

IGraphicsContext::~IGraphicsContext()
{
	if (s_pInstance)
	{
		delete s_pInstance;
		s_pInstance = nullptr;
	}
}
}


