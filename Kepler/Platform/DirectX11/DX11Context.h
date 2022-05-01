#pragma once

#include "Renderer/GraphicsContext.h"


namespace kepler {

	class DX11Context : public IGraphicsAPIContext
	{
	private:


	public:
		DX11Context();

		
		// Inherited via IRenderer
		~DX11Context();
		virtual void Init() override;
		virtual void Present() override;
	};
}