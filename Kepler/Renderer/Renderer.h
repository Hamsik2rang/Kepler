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
		
		// ������ ��ü�� �ʱ�ȭ�մϴ�.
		bool Init(const WindowData& data);
		// ������ ��ü�� ��ȯ�մϴ�.
		void Shutdown();
		// ��ü�� �������մϴ�.
		bool Render(float rotation);

		void ClearColor();
		void SetColor();
		void SetViewport();
		void Resize(uint32_t width, uint32_t height);
		// TODO: should be going to get buffer(vertex, index) params
		void DrawIndexed();
	};

}