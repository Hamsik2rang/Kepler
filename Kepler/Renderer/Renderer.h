#pragma once

#include "Core/Base.h"
#include "Renderer/GraphicsAPI.h"

namespace kepler {

	class WindowsWindow;
	class DX11Camera;
	class DX11Model;
	class DX11TextureShader;
	class Renderer
	{
	private:
		static Renderer* s_pInstance;
		HWND m_hWnd = nullptr;
		IGraphicsAPI* m_pGraphicsAPI = nullptr;
		DX11Camera* m_pCamera = nullptr;
		DX11TextureShader* m_pTextureShader = nullptr;
		DX11Model* m_pModel = nullptr;

		Renderer();
		~Renderer();
	public:
		static Renderer* Get();
		static void Init();
		
		inline eGraphicsAPI GetAPI() const { return m_pGraphicsAPI->GetAPI(); }
		
		// 렌더링합니다.
		bool Render(HWND hWnd);

		void ClearColor();
		void SetColor();
		void SetViewport();
		void Resize(uint32_t width, uint32_t height);
		// TODO: should be going to get buffer(vertex, index) params
		void DrawIndexed();
	};

}