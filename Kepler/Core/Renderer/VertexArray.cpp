#include "kepch.h"

#include "VertexArray.h"
#include "Core/Renderer/GraphicsAPI.h"
#include "Platform/DirectX11/DX11VertexArray.h"

namespace kepler {
	std::shared_ptr<IVertexArray> IVertexArray::Create()
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
				return std::make_shared<DX11VertexArray>();
			}
			//...
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}
}