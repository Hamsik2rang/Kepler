#include "EditorLayer.h"
#include "Kepler.h"

namespace kepler {

void EditorLayer::OnAttach()
{
	kepler::IFrameBuffer::Get()->AddGBuffer(0, 1);

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

	std::shared_ptr<kepler::IVertexBuffer> pVB = kepler::IVertexBuffer::Create(vertices, sizeof(vertices), kepler::EBufferUsage::Default);
	pVB->SetLayout({
		{"POSITION", 0, kepler::eShaderDataType::Float3, 0, sizeof(float) * 3},
		{"COLOR", 0, kepler::eShaderDataType::Float4, sizeof(float) * 3, sizeof(float) * 4}
		});

	std::shared_ptr<kepler::IIndexBuffer> pIB = kepler::IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), kepler::EBufferUsage::Default);

	auto pCubeVA = kepler::IVertexArray::Create();
	pCubeVA->AddVertexBuffer(pVB);
	pCubeVA->SetIndexBuffer(pIB);

	m_pCubeEntity = m_scene.CreateEntity();
	m_pCubeEntity->AddComponent<kepler::TransformComponent>(kepler::Vec3f{ 0.0f, 0.0f, 4.0f }, kepler::Vec3f{ 0.0f, 0.0f, 0.0f }, kepler::Vec3f{ 1.0f, 1.0f, 1.0f });
	m_pCubeEntity->AddComponent<kepler::MeshRendererComponent>(pCubeVA);
	m_pCubeEntity->AddComponent<kepler::TagComponent>("RainbowCube");
}

void EditorLayer::OnDetach()
{

}


void EditorLayer::OnUpdate(const float deltaTime)
{
	m_time += deltaTime;
	kepler::IRenderState::Get()->Bind();
	kepler::IFrameBuffer::Get()->BindGBuffer(0, 1);

	m_camera.OnUpdate(deltaTime);
}

void EditorLayer::OnRender()
{
	kepler::Renderer::Get()->BeginScene(m_camera);

	auto entities = m_scene.GetAllEntitiesWith<kepler::TransformComponent, kepler::MeshRendererComponent>();
	for (auto entity : entities)
	{
		auto transform = entity->GetComponent<kepler::TransformComponent>();
		auto mesh = entity->GetComponent<kepler::MeshRendererComponent>();

		kepler::Renderer::Get()->Submit(mesh->pVertexArray,
			kepler::math::GetScalingMatrix(transform->scale) *
			kepler::math::GetRotationMatrix(kepler::Quaternion::FromEuler(transform->rotation)) *
			kepler::math::GetTranslateMatrix(transform->translation));
	}
	auto height = kepler::Application::Get()->GetWindow().GetHeight();
	auto width = kepler::Application::Get()->GetWindow().GetWidth();
	kepler::Renderer::Get()->EndScene();
	
	kepler::Renderer::Get()->SetViewport(width, height, 0.0f, 1.0f);
	kepler::IFrameBuffer::Get()->BindColorBuffer();
}

void EditorLayer::OnGUIRender()
{
	m_dockspacePanel.OnImGuiRender();

	// Scene View Begin
	m_sceneViewPanel.OnImGuiRender();
	// Scene View End

	// Hierarchy View Begin
	m_hierarchyPanel.OnImGuiRender();
	// Hierarchy View End

	// Inspector View Begin
	m_inspectorPanel.OnImGuiRender();
	// Inspector View End
}

void EditorLayer::OnEvent(kepler::Event& e)
{
	m_camera.OnEvent(e);
}
}