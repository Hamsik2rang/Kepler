#pragma once

#include "Core/Base.h"
#include "Core/Window.h"
#include "Renderer/GraphicsAPI.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Camera.h"
#include "Platform/DirectX11/DX11Model.h"
#include "Platform/DirectX11/DX11TextureShader.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace kepler {

	class Renderer
	{
	private:
		static Renderer* s_pInstance;
		IGraphicsAPI* m_pGraphicsAPI;
		
		// º°Èñ ÄÚµå
		HWND m_hWnd = nullptr;
		Camera* m_pCamera = nullptr;
		DX11TextureShader* m_pTextureShader = nullptr;
		DX11Model* m_pModel = nullptr;

		XMMATRIX m_worldMatrix{};
		XMMATRIX m_projectionMatrix{};
		XMMATRIX m_orthoMatrix{};
		XMMATRIX m_viewMatrix{};

		Renderer();
		~Renderer();

	public:
		static Renderer* Get();
		static void Init();
		
		inline eGraphicsAPI GetAPI() const { return m_pGraphicsAPI->GetAPI(); }

		bool Render(IWindow* pWindow);

		void ClearColor();
		void SetColor();
		void SetViewport(const uint32_t width, const uint32_t height, const float minDepth, const float maxDepth);
		void Resize(uint32_t width, uint32_t height);
		
		// TODO: should be going to get buffer(vertex, index) params
		void Submit(std::shared_ptr<IVertexArray>& pVertexArray);
	};
}