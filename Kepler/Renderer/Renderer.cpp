#include "kepch.h"

#include "Renderer.h"
#include "GraphicsContext.h"
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
		m_pModel = new DX11Model();
		m_pTextureShader = new DX11TextureShader();

		m_pCamera->SetPosition(0.0f, 0.0f, -5.0f);

		// ���� �迭�� �����͸� �����մϴ�
		VertexType* vertices = new VertexType[6]; // m_pModel���� delete
		vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
		vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
		vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top left.
		vertices[1].texture = XMFLOAT2(0.0f, 0.0f);
		vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
		vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
		vertices[3].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
		vertices[3].texture = XMFLOAT2(1.0f, 1.0f);
		vertices[4].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top left.
		vertices[4].texture = XMFLOAT2(0.0f, 0.0f);
		vertices[5].position = XMFLOAT3(1.0f, 1.0f, 0.0f);  // Top right.
		vertices[5].texture = XMFLOAT2(1.0f, 0.0f);
		m_pModel->Init(((DX11Context*)IGraphicsContext::Get())->GetDevice(), vertices, 6, L"../8k_earth_daymap.dds");
	}

	Renderer::~Renderer()
	{
		m_pModel->Shutdown();
		delete m_pModel;
		delete m_pCamera;
	}

	bool Renderer::Render(WindowsWindow* pWWnd)
	{
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

		// 2D ������ ���� �� Z ���۸� ���ϴ�.
		pContext->TurnZBufferOff();

		// TODO: ������ �ڵ带 �ۼ��ϼ���...
		m_pModel->Render(pContext->GetDeviceContext());

		if (!m_pTextureShader->Init(IGraphicsContext::Get()->GetDevice(), m_hWnd))
		{
			MessageBox(m_hWnd, L"Could not initialize Texture Shader.", L"Error", MB_OK);
			return false;
		}

		if (!m_pTextureShader->Render(pContext->GetDeviceContext(), m_pModel->GetIndexCount(), m_worldMatrix,
			m_viewMatrix, m_projectionMatrix, m_pModel->GetTexture()))
		{
			return false;
		}

		// 2D ������ ���� �� Z ���۸� �մϴ�.
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
