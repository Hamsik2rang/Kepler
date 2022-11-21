#include "kepch.h"

#include "FrameBuffer.h"
#include "GraphicsAPI.h"
#include "Platform/DirectX11/DX11FrameBuffer.h"

namespace kepler {

std::shared_ptr<IFrameBuffer> IFrameBuffer::s_pInstance = nullptr;

std::shared_ptr<IFrameBuffer> IFrameBuffer::Get()
{
	if (!s_pInstance)
	{
		return Create();
	}

	return s_pInstance;
}

std::shared_ptr<IFrameBuffer> IFrameBuffer::Create()
{
	EGraphicsAPI api = IGraphicsAPI::GetAPI();
	switch (api)
	{
	case EGraphicsAPI::None:
	{
		KEPLER_CORE_ASSERT(false, "None Graphics API is not supported.");
	}
	break;
	case EGraphicsAPI::DirectX11:
	{
		s_pInstance = std::make_shared<DX11FrameBuffer>();
	}
	break;
	}
	return s_pInstance;
}
}