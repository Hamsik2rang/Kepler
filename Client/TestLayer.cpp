#include "kepch.h"

#include "TestLayer.h"
#include "Renderer/Renderer.h"
#include "Platform/DirectX11/DX11Context.h"

TestLayer::TestLayer()
	:Layer("Test")
{
	modelCount = 1;
	m_ppModel = new DX11Model * [modelCount];
	for (int i = 0; i < modelCount; i++)
	{
		m_ppModel[i] = new DX11Model();
	}

	// 정점 배열에 데이터를 설정합니다
	VertexType* vertices = new VertexType[6]; // m_pModel에서 delete
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
	m_ppModel[0]->Init(((DX11Context*)IGraphicsContext::Get())->GetDevice(), vertices, 6, L"../8k_earth_daymap.dds");
}

TestLayer::~TestLayer()
{
	for (int i = modelCount - 1; i >= 0; i--)
	{
		m_ppModel[i]->Shutdown();
		delete m_ppModel[i];
	}
	delete[] m_ppModel;
}

void TestLayer::OnAttach()
{

}

void TestLayer::OnDetach()
{

}


void TestLayer::OnUpdate()
{
	// TODO: Update Loop마다 해야할 작업들 작성
	if (!kepler::Renderer::Get()->Render(m_ppModel, 1))
	{
		KEPLER_CORE_CRITICAL("CRITICAL: Can't Rendering - {0} {1}", __FILE__, __LINE__);
		return;
	}
}

void TestLayer::OnEvent(kepler::Event& e)
{
	KEPLER_TRACE("{0}", e);
}