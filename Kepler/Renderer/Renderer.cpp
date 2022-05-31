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

	//bool Renderer::Render(IWindow* pWWnd)
	//{
	//	DX11Context* pContext = (DX11Context*)IGraphicsContext::Get();
	//	DX11API* pAPI = (DX11API*)m_pGraphicsAPI;

	//	// ����� �����մϴ�.

	//	// ���� ����� �׵� ��ķ� �ʱ�ȭ�մϴ�
	//	m_worldMatrix = XMMatrixIdentity();

	//	// 3D ������������ ���� ����� ����ϴ�
	//	float fieldOfView = XM_PI / 4.0f;
	//	float screenAspect = (float)pWWnd->GetWidth() / (float)pWWnd->GetHeight();
	//	float screenNear = 1000.0f;
	//	float screenDepth = 0.1f;
	//	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	//	// 2D ������������ ���� ���� ����� ����ϴ�
	//	m_orthoMatrix = XMMatrixOrthographicLH((float)pWWnd->GetWidth(), (float)pWWnd->GetHeight(), screenNear, screenDepth);

	//	// Camera�� �� ����� �����ɴϴ�.
	//	m_pCamera->Render();
	//	m_pCamera->GetViewMatrix(m_viewMatrix);

	//	// 2D ������ ���� �� Z ���۸� ���ϴ�.
	//	pContext->TurnZBufferOff();

	//	// TODO: ������ �ڵ带 �ۼ��ϼ���...
	//	m_pModel->Render(pContext->GetDeviceContext());

	//	if (!m_pTextureShader->Init(IGraphicsContext::Get()->GetDevice(), m_hWnd))
	//	{
	//		MessageBox(m_hWnd, L"Could not initialize Texture Shader.", L"Error", MB_OK);
	//		return false;
	//	}

	//	if (!m_pTextureShader->Render(pContext->GetDeviceContext(), m_pModel->GetIndexCount(), m_worldMatrix,
	//		m_viewMatrix, m_projectionMatrix, m_pModel->GetTexture()))
	//	{
	//		return false;
	//	}

	//	// 2D ������ ���� �� Z ���۸� �մϴ�.
	//	pContext->TurnZBufferOn();
	//	return true;
	//}

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

	void Renderer::Submit(std::shared_ptr<IVertexArray>& pVertexArray, const Mat44f& transform)
	{
		m_pGraphicsAPI->DrawIndexed(pVertexArray);
	}
}
