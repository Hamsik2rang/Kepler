#include "kepch.h"

#include "GraphicsAPI.h"
#include "Platform/DirectX11/DX11API.h"

namespace kepler {

	eGraphicsAPI IGraphicsAPI::s_API = eGraphicsAPI::DirectX11;

	IGraphicsAPI* IGraphicsAPI::Create()
	{
		switch (s_API)
		{
		case eGraphicsAPI::None: 
			KEPLER_CORE_ASSERT(false, "eGraphicsAPI::None is not supported");
			return nullptr;
		case eGraphicsAPI::DirectX11:
			return new DX11API;
		}

		KEPLER_CORE_ASSERT(false, "unknown API");
		return nullptr;
	}

}