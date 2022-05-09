#include "kepch.h"

#include "VertexArray.h"
#include "GraphicsAPI.h"
#include "Platform/DirectX11/DX11VertexArray.h"

namespace kepler {
	std::shared_ptr<IVertexArray> IVertexArray::Create()
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
				return std::make_shared<DX11VertexArray>();
			}
			//...
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}
}