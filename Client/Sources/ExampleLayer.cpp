#include "ExampleLayer.h"


using namespace kepler;

void ExampleLayer::OnAttach()
{
	m_pVertexArray = IVertexArray::Create();
	float vertices[]{
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};
	
	std::shared_ptr<IVertexBuffer> pVertex = IVertexBuffer::Create(vertices, sizeof(vertices), eBufferUsage::Default);
	BufferLayout layout = {
		{ "POSITION", 0, kepler::eShaderDataType::Float3, 0, sizeof(float) * 3},
	};
	pVertex->SetLayout(layout);
	m_pVertexArray->AddVertexBuffer(pVertex);

	//float color[]{
	//	1.0f, 0.0f, 0.0f, 1.0f,
	//	0.0f, 1.0f, 0.0f, 1.0f,
	//	0.0f, 0.0f, 1.0f, 1.0f,
	//	1.0f, 1.0f, 1.0f, 1.0f,
	//};
	//
	//std::shared_ptr<IVertexBuffer> pColor = IVertexBuffer::Create(color, sizeof(color), eBufferUsage::Default);
	//layout = { 
	//	{ "COLOR", 0, eShaderDataType::Float4, 0, sizeof(float) * 4 } 
	//};
	//pColor->SetLayout(layout);
	//m_pVertexArray->AddVertexBuffer(pColor);
	
	uint32_t indices[]{
		0, 2, 3,
		0, 3, 1
	};
	std::shared_ptr<IIndexBuffer> pIBuffer = IIndexBuffer::Create(indices, 6, eBufferUsage::Default);
	m_pVertexArray->SetIndexBuffer(pIBuffer);
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate() 
{
	//auto window = Application::Get()->GetWindow();
	//kepler::Renderer::Get()->Render(window);

	//XMMATRIX viewProjection = XMMatrixLookAtLH({ 0.0f, 1.0f, 5.0f, 0.0f }, { 0.0f, 0.0f, 0.0f , 0.0f }, { 0.0f, 1.0f, 0.0f , 0.0f })
	//	* XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1280 / 720, 0.01f, 100.0f);
	//XMMATRIX view = XMMatrixLookAtLH({ 0.0f, 1.0f, 5.0f, 0.0f }, { 0.0f, 0.0f, 0.0f , 0.0f }, { 0.0f, 1.0f, 0.0f , 0.0f });
	//XMMATRIX projection = XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1280 / 720, 0.01f, 100.0f);
	
	//ShaderCache::GetShader("VSSolid")->SetMatrix("g_ViewProjection", XMMatrixTranspose(viewProjection));
	//ShaderCache::GetShader("VSSolid")->SetMatrix("g_World", XMMatrixTranspose(XMMatrixIdentity()));
	//ShaderCache::GetShader("VSSolid")->SetMatrix("g_View", XMMatrixTranspose(view));
	//ShaderCache::GetShader("VSSolid")->SetMatrix("g_Projection", XMMatrixTranspose(projection));
	
	ShaderCache::GetShader("VSTest")->Bind();
	ShaderCache::GetShader("PSTest")->Bind();
	
	m_pVertexArray->Bind();
	Renderer::Get()->Submit(m_pVertexArray);
}

void ExampleLayer::OnEvent(Event& e)
{
	KEPLER_TRACE("{0}", e);
}