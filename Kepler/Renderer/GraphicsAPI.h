#pragma once

#include "Core/Base.h"
#include "Renderer/VertexArray.h"

namespace kepler {

	enum class eGraphicsAPI
	{
		None		= 0,
		DirectX11	= 1,
	};

	class IGraphicsAPI
	{
	private:
		static eGraphicsAPI s_API;

	public:
		virtual void Init() = 0;

		virtual void ClearColor() = 0;
		virtual void SetColor(const float color[4]) = 0;
		virtual void SetViewport(const uint32_t width, const uint32_t height, const float minDepth = 0.0f, const float maxDepth = 1.0f) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		// TODO: should be going to get buffer(vertex, index) params
		virtual void DrawIndexed(const std::shared_ptr<IVertexArray>& pVertexArray) = 0;

		inline static void SetAPI(eGraphicsAPI api) { s_API = api; }
		inline static eGraphicsAPI GetAPI() { return s_API; }
		static IGraphicsAPI* Create();
	};

}