#include "kepch.h"

#include "GraphicsAPI.h"
#include "Platform/DirectX11/DX11API.h"

namespace kepler {

EGraphicsAPI IGraphicsAPI::s_API = EGraphicsAPI::DirectX11;

IGraphicsAPI* IGraphicsAPI::Create()
{
	switch (s_API)
	{
	case EGraphicsAPI::None:
		KEPLER_CORE_ASSERT(false, "EGraphicsAPI::None is not supported");
		return nullptr;
	case EGraphicsAPI::DirectX11:
		return new DX11API;
	}

	KEPLER_CORE_ASSERT(false, "unknown API");
	return nullptr;
}

}