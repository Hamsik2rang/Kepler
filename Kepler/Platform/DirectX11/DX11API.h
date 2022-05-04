#pragma once

#include "Renderer/GraphicsAPI.h"

namespace kepler {

	class DX11API : public IGraphicsAPI
	{
		// Inherited via IGraphicsAPI
		virtual void Init() override;
		virtual void ClearColor() override;
		virtual void SetColor() override;
		virtual void SetViewport() override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void DrawIndexed() override;
	};

}