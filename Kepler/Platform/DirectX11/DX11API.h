#pragma once

#include "Renderer/GraphicsAPI.h"


namespace kepler {

	class DX11API : public IGraphicsAPI
	{
	private:
		float m_clearColor[4];

	public:
		DX11API();

		// Inherited via IGraphicsAPI
		virtual void Init() override;
		virtual void SetViewport(const uint32_t width, const uint32_t height, const float minDepth = 0.0f, const float maxDepth = 1.0f) override;
		virtual void DrawIndexed(const std::shared_ptr<IVertexArray>& pVertexArray) override;
		virtual void DrawIndexedInstanced(const std::shared_ptr<IVertexArray>& pVertexArray) override;
	};

}