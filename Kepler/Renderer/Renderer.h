#pragma once

#include "Core/Base.h"
#include "Renderer/GraphicsAPI.h"

namespace kepler {

	class Renderer
	{
	private:
		IGraphicsAPI* m_pGraphicsAPI;
		static Renderer* s_pInstance;
		Renderer();

	public:
		static Renderer* Get();
		static void Init();
		
		inline eGraphicsAPI GetAPI() const { return m_pGraphicsAPI->GetAPI(); }
		
		void ClearColor();
		void SetColor();
		void SetViewport();
		void Resize(uint32_t width, uint32_t height);
		// TODO: should be going to get buffer(vertex, index) params
		void DrawIndexed();
	};

}