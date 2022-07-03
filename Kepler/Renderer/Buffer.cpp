#include "kepch.h"

#include "Buffer.h"
#include "Platform/DirectX11/DX11Buffer.h"
#include "Renderer/GraphicsAPI.h"

namespace kepler {

	std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(const uint32_t size, eBufferUsage usage)
	{
		eGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case eGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not suppotted");
				return nullptr;
			}
		case eGraphicsAPI::DirectX11:
			{
				return std::make_shared<DX11VertexBuffer>(size, usage);
			}
			//...
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

	std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(const void* const vertices, uint32_t size, eBufferUsage usage)
	{
		eGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case eGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not suppotted");
				return nullptr;
			}
		case eGraphicsAPI::DirectX11:
			{
				return std::make_shared<DX11VertexBuffer>(vertices, size, usage);
			}
			//...
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

	std::shared_ptr<IIndexBuffer> IIndexBuffer::Create(const uint32_t* const indices, const uint32_t count, eBufferUsage usage)
	{
		eGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case eGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not suppotted");
				return nullptr;
			}
		case eGraphicsAPI::DirectX11:
			{
				return std::make_shared<DX11IndexBuffer>(indices, count);
			}
			//...
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

}