#include "kepch.h"

#include "Renderer.h"
#include "GraphicsContext.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/DirectX11/DX11Context.h"
#include "Platform/DirectX11/DX11API.h"
#include "Platform/DirectX11/DX11Model.h"
#include "Platform/DirectX11/DX11TextureShader.h"
#include "Renderer/Camera.h"

#include "Renderer/Shader.h"

namespace kepler {
	Renderer* Renderer::s_pInstance = nullptr;

	Renderer* Renderer::Get()
	{
		if (!s_pInstance)
		{
			s_pInstance = new Renderer;
		}
		return s_pInstance;
	}

	void Renderer::Init()
	{
		if (s_pInstance)
		{
			return;
		}
		s_pInstance = new Renderer;
	}

	Renderer::Renderer()
		:m_pGraphicsAPI{ IGraphicsAPI::Create() }
	{

	}

	Renderer::~Renderer()
	{

	}

	void Renderer::ClearColor()
	{
		m_pGraphicsAPI->ClearColor();
	}

	void Renderer::SetColor()
	{

	}

	void Renderer::SetViewport(const uint32_t width, const uint32_t height, const float minDepth, const float maxDepth)
	{
		m_pGraphicsAPI->SetViewport(width, height, minDepth, maxDepth);
	}

	void Renderer::Resize(uint32_t width, uint32_t height)
	{

	}

	void Renderer::Submit(std::shared_ptr<IVertexArray>& pVertexArray)
	{
		m_pGraphicsAPI->DrawIndexed(pVertexArray);
	}
}
