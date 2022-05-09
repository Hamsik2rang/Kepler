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

		// ���� �׸��� ���� ���۸� ����ϴ�
		pAPI->ClearColor();

		// ����� �����մϴ�.

		// ���� ����� �׵� ��ķ� �ʱ�ȭ�մϴ�
		m_worldMatrix = XMMatrixIdentity();

		// 3D ������������ ���� ����� ����ϴ�
		float fieldOfView = XM_PI / 4.0f;
		float screenAspect = (float)pWWnd->GetWidth() / (float)pWWnd->GetHeight();
		float screenNear = 1000.0f;
		float screenDepth = 0.1f;
		m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

		// 2D ������������ ���� ���� ����� ����ϴ�
		m_orthoMatrix = XMMatrixOrthographicLH((float)pWWnd->GetWidth(), (float)pWWnd->GetHeight(), screenNear, screenDepth);

		// Camera�� �� ����� �����ɴϴ�.
		m_pCamera->Render();
		m_pCamera->GetViewMatrix(m_viewMatrix);


		// TODO: ������ �ڵ带 �ۼ��ϼ���...
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
