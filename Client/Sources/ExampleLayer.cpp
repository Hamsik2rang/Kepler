#include "ExampleLayer.h"

bool ExampleLayer::OnKeyboardPressedEvent(kepler::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == kepler::key::F6)
	{
		auto rs = kepler::IRenderState::Get()->GetDepthState();
		rs.bDepthTest = true;
		rs.bDepthWrite = true;
		rs.comparer = kepler::eDepthComparer::Less;
		kepler::IRenderState::Get()->SetDepthState(rs);
	}
	if (e.GetKeyCode() == kepler::key::F7)
	{
		auto rs = kepler::IRenderState::Get()->GetDepthState();
		rs.bDepthTest = false;
		rs.bDepthWrite = false;
		rs.comparer = kepler::eDepthComparer::Never;
		kepler::IRenderState::Get()->SetDepthState(rs);
	}
	if (e.GetKeyCode() == kepler::key::F8)
	{
		auto rs = kepler::IRenderState::Get()->GetRasterizerState();
		rs.bWireFrame = true;
		kepler::IRenderState::Get()->SetRasterizerState(rs);
	}
	if (e.GetKeyCode() == kepler::key::F9)
	{
		auto rs = kepler::IRenderState::Get()->GetRasterizerState();
		rs.bWireFrame = false;
		kepler::IRenderState::Get()->SetRasterizerState(rs);
	}

	return true;
}

void ExampleLayer::OnAttach()
{
	kepler::ShaderStateDescription desc{};
	desc.pVertexShader = kepler::ShaderCache::Load(kepler::eShaderType::Vertex, "../Kepler/Resources/Shaders/HLSL/VSSolid.hlsl");
	desc.pPixelShader = kepler::ShaderCache::Load(kepler::eShaderType::Pixel, "../Kepler/Resources/Shaders/HLSL/PSSolid.hlsl");

	kepler::IRenderState::Get()->SetShaderState(desc);

	float vertices[]{
		-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 0.5f,
		1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 0.5f,
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 0.5f,
		-1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 0.5f,

		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f, 0.5f,
		1.0f, -1.0f, -1.0f,		0.0f, 1.0f, 0.0f, 0.5f,
		1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 0.5f,
		-1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 0.5f,

		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 0.5f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f, 0.5f,
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f, 1.0f, 0.5f,
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 0.5f,

		1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 0.5f,
		1.0f, -1.0f, -1.0f,		0.0f, 1.0f, 1.0f, 0.5f,
		1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 1.0f, 0.5f,
		1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 0.5f,

		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 1.0f, 0.5f,
		1.0f, -1.0f, -1.0f,		1.0f, 0.0f, 1.0f, 0.5f,
		1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 1.0f, 0.5f,
		-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 1.0f, 0.5f,

		-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 0.5f,
		1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 0.5f,
		1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 0.5f,
		-1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 0.5f,
	};

	uint32_t indices[]{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
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

	auto rs = kepler::IRenderState::Get()->GetRasterizerState();
	rs.bWireFrame = true;
	rs.cullMode = kepler::eCullMode::Front;
	rs.bIsFrontClockwise = false;
	kepler::IRenderState::Get()->SetRasterizerState(rs);

	auto ds = kepler::IRenderState::Get()->GetDepthState();
	ds.bDepthTest = true;
	ds.bDepthWrite = true;
	ds.comparer = kepler::eDepthComparer::Less;
	kepler::IRenderState::Get()->SetDepthState(ds);
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnEvent(kepler::Event& e)
{
	kepler::EventDispatcher dispatch(e);
	dispatch.Dispatch<kepler::KeyPressedEvent>(std::bind(&ExampleLayer::OnKeyboardPressedEvent, this, std::placeholders::_1));
	m_camera.OnEvent(e);
}

void ExampleLayer::OnUpdate(const float deltaTime)
{
	m_time += deltaTime;
	kepler::IRenderState::Get()->Bind();
	m_camera.OnUpdate(deltaTime);
}

void ExampleLayer::OnRender()
{
	kepler::Renderer::Get()->BeginScene(m_camera);

	kepler::Renderer::Get()->Submit(m_pVertexArray, kepler::math::GetRotationMatrixX(m_time) * kepler::math::GetRotationMatrixY(m_time / 3.0f) * kepler::math::GetTranslateMatrix({ 0.0f, 0.0f, 4.0f }));
	kepler::Renderer::Get()->Submit(m_pVertexArray, kepler::math::GetScalingMatrix({ 2.5f, 2.5f, 2.5f }) * kepler::math::GetRotationMatrixX(m_time / 2.0f) * kepler::math::GetRotationMatrixZ(m_time / 1.8f) * kepler::math::GetTranslateMatrix({ 0.0f, 0.0f, 60.0f }));

	kepler::Renderer::Get()->EndScene();
}