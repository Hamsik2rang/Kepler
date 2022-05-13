#include "ExampleLayer.h"

using namespace kepler;

void ExampleLayer::OnAttach()
{
	float vertices[]{
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};
	
	std::shared_ptr<IVertexBuffer>pVertex = IVertexBuffer::Create(vertices, sizeof(vertices), eBufferUsage::Default);
	BufferLayout layout = { 
		{"POSITION", 0, eShaderDataType::Float3, 0, sizeof(float) * 3}
	};
	pVertex->SetLayout(layout);

	float uv[]{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	std::shared_ptr<IVertexBuffer> pUV = IVertexBuffer::Create(uv, sizeof(uv), eBufferUsage::Default);
	layout = { { "TEXCOORD", 0, eShaderDataType::Float2, 0, sizeof(float) * 2 } };
	pUV->SetLayout(layout);
	
	uint32_t indices[]{
		0, 2, 3,
		0, 3, 1
	};
	std::shared_ptr<IIndexBuffer> pIBuffer = IIndexBuffer::Create(indices, sizeof(uint32_t) * 6, eBufferUsage::Default);

	std::shared_ptr<IVertexArray> m_pVertexArray = IVertexArray::Create();

	m_pVertexArray->AddVertexBuffer(pVertex);
	m_pVertexArray->SetIndexBuffer(pIBuffer);
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate() 
{
	auto window = kepler::Application::Get()->GetWindow();
	//kepler::Renderer::Get()->Render(window);

	XMMATRIX viewProjection = XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1280 / 720, 0.01f, 100.0f)
		* XMMatrixLookAtLH({ 0.0f, 1.0f, -5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

	ShaderCache::GetShader("VSTexture")->SetMatrix("g_ViewProjection", XMMatrixTranspose(viewProjection));
	ShaderCache::GetShader("VSTexture")->SetMatrix("g_World", XMMatrixTranspose(XMMatrixIdentity()));

	ShaderCache::GetShader("VSTexture")->Bind();
	ShaderCache::GetShader("PSTexture")->Bind();

	m_pVertexArray.get()->Bind();
	Renderer::Get()->Submit(m_pVertexArray);

}

void ExampleLayer::OnEvent(kepler::Event& e)
{
	KEPLER_TRACE("{0}", e);
}