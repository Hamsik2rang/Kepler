#include "kepch.h"

#include "Buffer.h"
#include "Platform/DirectX11/DX11Buffer.h"
#include "Renderer/GraphicsAPI.h"

namespace kepler {

	std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(uint32_t size)
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
				return std::make_shared<DX11VertexBuffer>(size);
			}
			//...
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

	std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(const float* const vertices, uint32_t size)
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
				return std::make_shared<DX11VertexBuffer>(vertices, size);
			}
			//...
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}



	std::shared_ptr<IIndexBuffer> IIndexBuffer::Create(const uint32_t* const indices, uint32_t size)
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
				return std::make_shared<DX11IndexBuffer>(indices, size);
			}
			//...
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

}