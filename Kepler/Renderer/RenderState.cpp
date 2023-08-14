#include "kepch.h"

#include "RenderState.h"
#include "GraphicsAPI.h"
#include "Platform/DirectX11/DX11State.h"


namespace kepler {

	std::shared_ptr<IRenderState>	IRenderState::s_pInstance = nullptr;
	ShaderStateDescription			IRenderState::s_shaderDesc{};
	RasterizerStateDescription		IRenderState::s_rasterizerDesc{};
	DepthStateDescription			IRenderState::s_depthDesc{};
	StencilStateDescription			IRenderState::s_stencilDesc{};
	BlendStateDescription			IRenderState::s_blendDesc{};

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