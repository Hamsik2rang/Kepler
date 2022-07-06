#include "ExampleLayer.h"

void ExampleLayer::OnAttach()
{
	kepler::ShaderCache::Load(kepler::eShaderType::Vertex, "../Kepler/Resources/Shaders/HLSL/VSTest.hlsl");
	kepler::ShaderCache::Load(kepler::eShaderType::Pixel, "../Kepler/Resources/Shaders/HLSL/PSTest.hlsl");

	float vertices[]{
		-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
	};

	uint32_t indices[]{
		0, 1, 2,
		0, 2, 3
	};

	std::shared_ptr<kepler::IVertexBuffer> pVB = kepler::IVertexBuffer::Create(vertices, sizeof(vertices), kepler::eBufferUsage::Default);
	pVB->SetLayout({
		{"POSITION", 0, kepler::eShaderDataType::Float3, 0, sizeof(float) * 3},
		{"COLOR", 0, kepler::eShaderDataType::Float4, sizeof(float) * 3, sizeof(float) * 4}
		});

	std::shared_ptr<kepler::IIndexBuffer> pIB = kepler::IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), kepler::eBufferUsage::Default);
	
	m_pVertexArray = kepler::IVertexArray::Create();
	m_pVertexArray->AddVertexBuffer(pVB);
	m_pVertexArray->SetIndexBuffer(pIB);
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnEvent(kepler::Event& e)
{

}

void ExampleLayer::OnUpdate(const float deltaTime)
{
	m_time += deltaTime;
	kepler::ShaderCache::GetShader("VSTest")->Bind();
	kepler::ShaderCache::GetShader("PSTest")->Bind();

	kepler::ShaderCache::GetShader("PSTest")->SetFloat("g_Time", m_time);

	m_pVertexArray->Bind();
	kepler::Renderer::Get()->Submit(m_pVertexArray, kepler::Mat44f::Identity);
}