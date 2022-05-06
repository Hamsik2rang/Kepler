#include "kepch.h"

#include "Renderer.h"
#include "GraphicsContext.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/DirectX11/DX11Context.h"
#include "Platform/DirectX11/DX11API.h"
#include "Platform/DirectX11/DX11Camera.h"

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
		if (!s_pInstance)
		{
			s_pInstance = new Renderer;
		}
	}

	Renderer::Renderer()
		:m_pGraphicsAPI{ nullptr }
	{
		m_pGraphicsAPI = IGraphicsAPI::Create();
	}

	bool Renderer::Render(float rotation)
	{
		DX11Context* pContext = (DX11Context*)IGraphicsContext::Get();
		DX11API* pAPI = (DX11API*)IGraphicsAPI::GetAPI();

		// ���� �׸��� ���� ���۸� ����ϴ�
		pAPI->ClearColor();

		// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
		XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
		m_pCamera->Render();
		m_pCamera->GetViewMatrix(viewMatrix);
		pContext->GetWorldMatrix(worldMatrix);
		pContext->GetProjectionMatrix(projectionMatrix);
		pContext->GetOrthoMatrix(orthoMatrix);

		// 2D ������ ���� �� Z ���۸� ���ϴ�.
		pContext->TurnZBufferOff();

		// TODO: ������ �ڵ带 �ۼ��ϼ���...

		// 2D ������ ���� �� Z ���۸� �մϴ�.
		pContext->TurnZBufferOn();
		return true;
	}

	bool Renderer::Init(const WindowData& data)
	{
		if (!IGraphicsContext::Get()->Init(data))
		{
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Initialize IGraphicsContext");
			KEPLER_ASSERT(false, "Can't Initialize IGraphicsContext");
		}
		return true;
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::ClearColor()
	{
		m_pGraphicsAPI->ClearColor();
	}

	void Renderer::SetColor()
	{

	}

	void Renderer::SetViewport()
	{

	}

	void Renderer::Resize(uint32_t width, uint32_t height)
	{

	}

	void Renderer::DrawIndexed()
	{

	}
}
