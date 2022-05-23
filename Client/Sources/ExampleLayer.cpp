#include "ExampleLayer.h"


using namespace kepler;

void ExampleLayer::OnAttach()
{
	ShaderCache::Load(eShaderType::Vertex, "../Kepler/Resources/Shaders/HLSL/VSTest.hlsl");
	ShaderCache::Load(eShaderType::Pixel, "../Kepler/Resources/Shaders/HLSL/PSTest.hlsl");

	//---------------------------------------------------
	m_pVertexArray = IVertexArray::Create();
	float vertices[] = {
		 1.0f,  1.0f, 0.0f,  // top right
		 1.0f, -1.0f, 0.0f,  // bottom right
		-1.0f, -1.0f, 0.0f,  // bottom left
		-1.0f,  1.0f, 0.0f   // top left
	};

	std::shared_ptr<IVertexBuffer> pVertex = IVertexBuffer::Create(vertices, sizeof(vertices), eBufferUsage::Default);
	BufferLayout layout = {
		{ "POSITION", 0, kepler::eShaderDataType::Float3, 0, sizeof(float) * 3},
	};
	pVertex->SetLayout(layout);
	m_pVertexArray->AddVertexBuffer(pVertex);

	float colors[]{
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
	};
	std::shared_ptr<IVertexBuffer> pColor = IVertexBuffer::Create(colors, sizeof(colors), eBufferUsage::Default);
	layout = { { "COLOR", 0, kepler::eShaderDataType::Float4, 0, sizeof(float) * 4 } };
	pColor->SetLayout(layout);
	m_pVertexArray->AddVertexBuffer(pColor);

	//---------------------------------------------------
	uint32_t indices[] = {
		0, 1, 3,	// first triangle
		1, 2, 3		// second triangle
	};

	std::shared_ptr<IIndexBuffer> pIBuffer = IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), eBufferUsage::Default);
	m_pVertexArray->SetIndexBuffer(pIBuffer);

	m_timer.Start();
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate()
{
	float curTime = m_timer.Elapsed();

	ShaderCache::GetShader("VSTest")->Bind();
	ShaderCache::GetShader("PSTest")->Bind();
	
	ShaderCache::GetShader("PSTest")->SetFloat("g_Time", curTime);
	
	m_pVertexArray->Bind();
	
	Renderer::Get()->Submit(m_pVertexArray);
}

void ExampleLayer::OnEvent(Event& e)
{
	KEPLER_TRACE("{0}", e);
}