#pragma once

#include "Core/Base.h"
#include "Renderer/GraphicsAPI.h"

namespace kepler {

	struct WindowData;
	class DX11Camera;

	class Renderer
	{
	private:
		IGraphicsAPI* m_pGraphicsAPI = nullptr;
		static Renderer* s_pInstance = nullptr;
		DX11Camera* m_pCamera = nullptr;

		Renderer();
	public:
		static Renderer* Get();
		static void Init();
		
		inline eGraphicsAPI GetAPI() const { return m_pGraphicsAPI->GetAPI(); }
		
		// 렌더링 객체를 초기화합니다.
		bool Init(const WindowData& data);
		// 렌더링 객체를 반환합니다.
		void Shutdown();
		// 객체를 렌더링합니다.
		bool Render(float rotation);

		void ClearColor();
		void SetColor();
		void SetViewport();
		void Resize(uint32_t width, uint32_t height);
		// TODO: should be going to get buffer(vertex, index) params
		void DrawIndexed();
	};

}