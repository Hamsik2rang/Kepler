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
		virtual void ClearColor() override;
		virtual void SetColor(const float color[4]) override;
		virtual void SetViewport(const uint32_t width, const uint32_t height, const float minDepth = 0.0f, const float maxDepth = 1.0f) override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void DrawIndexed(const std::shared_ptr<IVertexArray>& pVertexArray) override;
	};

}