#pragma once

#include "Core/Base.h"
#include "Renderer/GraphicsAPI.h"

namespace kepler {

	class WindowsWindow;
	class DX11Camera;
	class DX11Model;

	class Renderer
	{
	private:
		static Renderer* s_pInstance;
		IGraphicsAPI* m_pGraphicsAPI = nullptr;
		DX11Camera* m_pCamera = nullptr;

		DX11Model* m_pModel = nullptr;

		Renderer();
		~Renderer();
	public:
		static Renderer* Get();
		static void Init();
		
		inline eGraphicsAPI GetAPI() const { return m_pGraphicsAPI->GetAPI(); }
		
		// 렌더링합니다.
		bool Render();

		void ClearColor();
		void SetColor();
		void SetViewport();
		void Resize(uint32_t width, uint32_t height);
		// TODO: should be going to get buffer(vertex, index) params
		void DrawIndexed();
	};

}