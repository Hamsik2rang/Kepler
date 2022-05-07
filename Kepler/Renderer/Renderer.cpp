#include "kepch.h"

#include "Renderer.h"
#include "GraphicsContext.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/DirectX11/DX11Context.h"
#include "Platform/DirectX11/DX11API.h"
#include "Platform/DirectX11/DX11Camera.h"
#include "Platform/DirectX11/DX11Model.h"

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
		m_pCamera = new DX11Camera();
		m_pModel = new DX11Model();

		m_pCamera->SetPosition(0.0f, 0.0f, -5.0f);

		/*
		// ���� �迭�� �����͸� �����մϴ�
		VertexType* vertices = new VertexType[3]; // m_pModel���� delete
		vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
		vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
		vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.

		m_pModel->Init(((DX11Context*)IGraphicsContext::Get())->GetDevice(), vertices, 3, L"8k_earth_daymap.dds");
		*/
	}

	Renderer::~Renderer()
	{
		m_pModel->Shutdown();
		delete m_pModel;
		delete m_pCamera;
	}

	bool Renderer::Render()
	{
		DX11Context* pContext = (DX11Context*)IGraphicsContext::Get();
		DX11API* pAPI = (DX11API*)m_pGraphicsAPI;

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
