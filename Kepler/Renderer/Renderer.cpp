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
		m_pCamera = new DX11Camera;
	}

	Renderer::~Renderer()
	{
		delete m_pCamera;
	}

	bool Renderer::Render()
	{
		DX11Context* pContext = (DX11Context*)IGraphicsContext::Get();
		DX11API* pAPI = (DX11API*)m_pGraphicsAPI;

		// 씬을 그리기 위해 버퍼를 지웁니다
		pAPI->ClearColor();

#ifndef DEBUG
		// 디버그용 코드 (뷰포트를 흰색으로 만듭니다.)
		float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		pAPI->SetColor(color);
#endif // DEBUG

		// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
		XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
		m_pCamera->Render();
		m_pCamera->GetViewMatrix(viewMatrix);
		pContext->GetWorldMatrix(worldMatrix);
		pContext->GetProjectionMatrix(projectionMatrix);
		pContext->GetOrthoMatrix(orthoMatrix);

		// 2D 렌더링 시작 시 Z 버퍼를 끕니다.
		pContext->TurnZBufferOff();

		// TODO: 렌더링 코드를 작성하세요...

		// 2D 렌더링 종료 시 Z 버퍼를 켭니다.
		pContext->TurnZBufferOn();
		return true;
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
