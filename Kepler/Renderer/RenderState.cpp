#include "kepch.h"

#include "RenderState.h"
#include "GraphicsAPI.h"
#include "Platform/DirectX11/DX11State.h"


namespace kepler {

	std::shared_ptr<IRenderState> IRenderState::s_pInstance = nullptr;

	std::shared_ptr<IRenderState> IRenderState::Create()
	{
		if (s_pInstance)
		{
			return s_pInstance;
		}

		eGraphicsAPI eAPI = IGraphicsAPI::GetAPI();
		switch (eAPI)
		{
		case eGraphicsAPI::None:
			{
				KEPLER_CORE_ASSERT(false, "None API not supported");
			}
			break;
		case eGraphicsAPI::DirectX11:
			{
				s_pInstance = std::make_shared<DX11State>();
			}
			break;
		default:
			{
				KEPLER_CORE_ASSERT(false, "Invalid Graphics API");
			}
			break;
		}

		return s_pInstance;
	}

}