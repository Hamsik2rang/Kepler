#include "kepch.h"

#include "Renderer.h"
#include "GraphicsContext.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/DirectX11/DX11Context.h"
#include "Platform/DirectX11/DX11API.h"
#include "Renderer/Camera.hpp"
#include "Renderer/RenderState.h"

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
	: m_pGraphicsAPI{ IGraphicsAPI::Create() }
{
	IRenderState::Create();

}

Renderer::~Renderer()
{

}

void Renderer::SetViewport(const uint32_t width, const uint32_t height, const float minDepth, const float maxDepth)
{
	m_pGraphicsAPI->SetViewport(width, height, minDepth, maxDepth);
}

void Renderer::BeginScene(Camera& camera)
{
	Mat44f view = camera.GetViewMatrix();
	Mat44f proj = camera.GetProjectionMatrix();
	Mat44f viewProj = camera.GetViewProjectionMatrix();
	auto shaderDesc = IRenderState::Get()->GetShaderState();

	shaderDesc.pVertexShader->SetMatrix("g_View", view.Transpose());
	shaderDesc.pVertexShader->SetMatrix("g_Projection", proj.Transpose());
	shaderDesc.pVertexShader->SetMatrix("g_ViewProjection", viewProj.Transpose());
}

void Renderer::EndScene()
{

}

void Renderer::Submit(std::shared_ptr<IVertexArray>& pVertexArray, const Mat44f& transform)
{
	auto shaderDesc = IRenderState::Get()->GetShaderState();
	//shaderDesc.pVertexShader->SetMatrix("g_World", transform);
	m_pGraphicsAPI->DrawIndexed(pVertexArray);
}
}
