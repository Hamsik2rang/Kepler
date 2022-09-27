#include "kepch.h"

#include "Buffer.h"
#include "Platform/DirectX11/DX11Buffer.h"
#include "Core/Renderer/GraphicsAPI.h"

namespace kepler {

	std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(const uint32_t size, EBufferUsage usage)
	{
		EGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case EGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not suppotted");
				return nullptr;
			}
		case EGraphicsAPI::DirectX11:
			{
				return std::make_shared<DX11VertexBuffer>(size, usage);
			}
			//...
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

	std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(const void* const vertices, uint32_t size, EBufferUsage usage)
	{
		EGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case EGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not suppotted");
				return nullptr;
			}
		case EGraphicsAPI::DirectX11:
			{
				return std::make_shared<DX11VertexBuffer>(vertices, size, usage);
			}
			//...
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

	std::shared_ptr<IIndexBuffer> IIndexBuffer::Create(const uint32_t* const indices, const uint32_t count, EBufferUsage usage)
	{
		EGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case EGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not suppotted");
				return nullptr;
			}
		case EGraphicsAPI::DirectX11:
			{
				return std::make_shared<DX11IndexBuffer>(indices, count);
			}
			//...
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

	std::shared_ptr<IInstanceBuffer> IInstanceBuffer::Create(const uint32_t count, const uint32_t size, EBufferUsage usage)
	{
		EGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case EGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not suppotted");
				return nullptr;
			}
		case EGraphicsAPI::DirectX11:
			{
				return std::make_shared<DX11InstanceBuffer>(count, size, usage);
			}
			//...
		}
		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

	std::shared_ptr<IInstanceBuffer> IInstanceBuffer::Create(const void* const data, const uint32_t count, const uint32_t size, EBufferUsage usage)
	{
		EGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case EGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not suppotted");
				return nullptr;
			}
		case EGraphicsAPI::DirectX11:
			{
				return std::make_shared<DX11InstanceBuffer>(data, count, size, usage);
			}
			//...
		}
		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}
}