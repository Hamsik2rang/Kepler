#pragma once

#include "Core/Base.h"
#include "Core/Window.h"
#include "Renderer/GraphicsAPI.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace kepler {

	class WindowsWindow;
	class Camera;
	class DX11Model;
	class DX11TextureShader;

	class Renderer
	{
	private:
		static Renderer* s_pInstance;
		
		IGraphicsAPI* m_pGraphicsAPI;

		Renderer();
		~Renderer();

	public:
		static Renderer* Get();
		static void Init();
		
		inline eGraphicsAPI GetAPI() const { return m_pGraphicsAPI->GetAPI(); }

		void ClearColor();
		void SetColor();
		void SetViewport(const uint32_t width, const uint32_t height, const float minDepth, const float maxDepth);
		void Resize(uint32_t width, uint32_t height);
		
		// TODO: should be going to get buffer(vertex, index) params
		void Submit(std::shared_ptr<IVertexArray>& pVertexArray);
	};

}