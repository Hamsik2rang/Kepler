#include "kepch.h"

#include "Renderer.h"
#include "GraphicsContext.h"
#include "Core/Application.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/DirectX11/DX11Context.h"
#include "Platform/DirectX11/DX11API.h"
#include "Platform/DirectX11/DX11Model.h"
#include "Platform/DirectX11/DX11TextureShader.h"
#include "Renderer/Camera.h"

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
		m_pCamera = new Camera();
		m_pTextureShader = new DX11TextureShader();

		m_pCamera->SetPosition(0.0f, 0.0f, -5.0f);
	}

	Renderer::~Renderer()
	{
		delete m_pCamera;
	}

	bool Renderer::Render(DX11Model** ppModel, int modelCount)
	{
		WindowsWindow* pWWnd = (WindowsWindow*)Application::Get()->GetWindow();
		DX11Context* pContext = (DX11Context*)IGraphicsContext::Get();
		DX11API* pAPI = (DX11API*)m_pGraphicsAPI;

		// 씬을 그리기 위해 버퍼를 지웁니다
		pAPI->ClearColor();

		// 행렬을 설정합니다.

		// 세계 행렬을 항등 행렬로 초기화합니다
		m_worldMatrix = XMMatrixIdentity();

		// 3D 렌더링을위한 투영 행렬을 만듭니다
		float fieldOfView = XM_PI / 4.0f;
		float screenAspect = (float)pWWnd->GetWidth() / (float)pWWnd->GetHeight();
		float screenNear = 1000.0f;
		float screenDepth = 0.1f;
		m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

		// 2D 렌더링을위한 직교 투영 행렬을 만듭니다
		m_orthoMatrix = XMMatrixOrthographicLH((float)pWWnd->GetWidth(), (float)pWWnd->GetHeight(), screenNear, screenDepth);

		// Camera로 뷰 행렬을 가져옵니다.
		m_pCamera->Render();
		m_pCamera->GetViewMatrix(m_viewMatrix);


		// TODO: 렌더링 코드를 작성하세요...
		if (!m_pTextureShader->Init(IGraphicsContext::Get()->GetDevice(), m_hWnd))
		{
			MessageBox(m_hWnd, L"Could not initialize Texture Shader.", L"Error", MB_OK);
			return false;
		}

		for (int i = 0; i < modelCount; i++)
		{
			ppModel[i]->Render(pContext->GetDeviceContext());
			if (!m_pTextureShader->Render(pContext->GetDeviceContext(), ppModel[i]->GetIndexCount(), m_worldMatrix,
				m_viewMatrix, m_projectionMatrix, ppModel[i]->GetTexture()))
			{
				return false;
			}
		}

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
