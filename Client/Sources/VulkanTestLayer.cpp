#include "VulkanTestLayer.h"


VulkanTestLayer::VulkanTestLayer()
{

}

VulkanTestLayer::~VulkanTestLayer()
{

}

void VulkanTestLayer::OnAttach()
{
	kepler::ShaderCache::Load(kepler::eShaderType::Vertex, "../Resources/Shaders/HLSL/VSSupernova.hlsl");
	kepler::ShaderCache::Load(kepler::eShaderType::Pixel, "../Resources/Shaders/HLSL/PSSupernova.hlsl");


	struct Vertex
	{
		kepler::Vec3f position;
		kepler::Vec3f normal;
		kepler::Vec2f uv;
	};

	Vertex vertices[4]{
		{ { -1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
		{ { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
		{ { 1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },
		{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } }
	};

	m_pVertexArray = kepler::IVertexArray::Create();

	m_pVertexBuffer = kepler::IVertexBuffer::Create(reinterpret_cast<float*>(vertices), sizeof(vertices), kepler::eBufferUsage::Default);
	m_pVertexBuffer->SetLayout({
		{"POSITION", 0, kepler::eShaderDataType::Float3, 0, sizeof(float) * 3},
		{"NORMAL", 0, kepler::eShaderDataType::Float3, sizeof(float) * 3, sizeof(float) * 3},
		{"TEXCOORD", 0, kepler::eShaderDataType::Float2, sizeof(float) * 3 + sizeof(float) * 3, sizeof(float) * 2}
		});

	m_pVertexArray->AddVertexBuffer(m_pVertexBuffer);

	uint32 indices[]{
		0, 1, 2, 
		0, 2, 3 
	};

	m_pIndexBuffer = kepler::IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32), kepler::eBufferUsage::Default);
	m_pVertexArray->SetIndexBuffer(m_pIndexBuffer);


	kepler::ShaderCache::GetShader("VSSupernova")->Bind();
	kepler::ShaderCache::GetShader("PSSupernova")->Bind();

	m_pTexture0 = kepler::ITexture2D::Create(kepler::eTextureDataType::UNorm_RGBA8, "../Resources/shadertoytexture0.jpg");
	m_pTexture0->Bind(0);

	m_pSampler = kepler::ISampler::Create(
		kepler::eTexelComparer::None, 
		kepler::eSamplerFilter::Linear, 
		kepler::eSamplerFilter::Linear,
		kepler::eSamplerFilter::Linear,
		kepler::eSamplerAddress::Wrap, 
		kepler::eSamplerAddress::Wrap, 
		kepler::eSamplerAddress::Wrap, 
		kepler::eSamplerComparer::Never, 
		0.0f, 
		1000.0f
	);
	m_pSampler->Bind(0);
	m_timer.Start();
}

void VulkanTestLayer::OnDetach()
{

};

void VulkanTestLayer::OnUpdate(float deltaTime)
{
	float time = m_timer.Elapsed();
	
	kepler::ShaderCache::GetShader("PSSupernova")->SetFloat("iTime", time);
	m_pVertexArray->Bind();

	kepler::Renderer::Get()->Submit(m_pVertexArray);
}

void VulkanTestLayer::OnRender()
{

}

void VulkanTestLayer::OnGUIRender()
{

}

void VulkanTestLayer::OnEvent(kepler::Event & e)
{

}
