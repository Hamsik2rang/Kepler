#include "EditorLayer.h"
#include <d3d11.h>

namespace kepler {

	void EditorLayer::OnAttach()
	{
		IFrameBuffer::Get()->AddGBuffer(0, 1);

		ShaderStateDescription desc{};
		desc.pVertexShader = ShaderCache::Load(EShaderType::Vertex, "../Kepler/Resources/Shaders/HLSL/VSSolid.hlsl");
		desc.pPixelShader = ShaderCache::Load(EShaderType::Pixel, "../Kepler/Resources/Shaders/HLSL/PSSolid.hlsl");

		IRenderState::Get()->SetShaderState(desc);

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

		std::shared_ptr<IVertexBuffer> pVB = IVertexBuffer::Create(vertices, sizeof(vertices), EBufferUsage::Default);
		pVB->SetLayout({
			{"POSITION", 0, EShaderDataType::Float3, 0, sizeof(float) * 3},
			{"COLOR", 0, EShaderDataType::Float4, sizeof(float) * 3, sizeof(float) * 4}
			});

		std::shared_ptr<IIndexBuffer> pIB = IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), EBufferUsage::Default);

		auto pCubeVA = IVertexArray::Create();
		pCubeVA->AddVertexBuffer(pVB);
		pCubeVA->SetIndexBuffer(pIB);

		m_pCubeEntity = m_scene.CreateEntity();
		m_pCubeEntity->AddComponent<TransformComponent>(Vec3f{ 0.0f, 0.0f, 4.0f }, Vec3f{ 0.0f, 0.0f, 0.0f }, Vec3f{ 1.0f, 1.0f, 1.0f });
		m_pCubeEntity->AddComponent<MeshRendererComponent>(pCubeVA);
		m_pCubeEntity->AddComponent<TagComponent>("Test Cube");
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(const float deltaTime)
	{
		m_time += deltaTime;
		IRenderState::Get()->Bind();
		if (m_sceneViewPanel.IsViewportResized())
		{
			Vec2f viewportSize = m_sceneViewPanel.GetViewportSize();
			// 1. FrameBuffer(Scene RenderTarget) Resizing and clear it's background.
			IFrameBuffer::Get()->ResizeGBuffer(0, 1, (uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			float col[]{ 0.1f, 0.1f, 0.1f, 1.0f };
			IFrameBuffer::Get()->ClearGBuffer(0, 1, col);
			// 2. Set Aspect
			m_camera.SetAspect(viewportSize.x / viewportSize.y);
		}

		IFrameBuffer::Get()->BindGBuffer(0, 1);

		m_camera.OnUpdate(deltaTime);
	}

	void EditorLayer::OnRender()
	{
		Vec2f viewportSize = m_sceneViewPanel.GetViewportSize();
		Renderer::Get()->SetViewport((uint32_t)viewportSize.x, (uint32_t)viewportSize.y, 0.0f, 1.0f);
		Renderer::Get()->BeginScene(m_camera);

		auto entities = m_scene.GetAllEntitiesWith<TransformComponent, MeshRendererComponent>();
		for (auto entity : entities)
		{
			auto transform = entity->GetComponent<TransformComponent>();
			auto mesh = entity->GetComponent<MeshRendererComponent>();

			Renderer::Get()->Submit(mesh->pVertexArray,
				math::GetScalingMatrix(transform->scale) *
				math::GetRotationMatrix(Quaternion::FromEuler(transform->rotation)) *
				math::GetTranslateMatrix(transform->translation));
		}

		Renderer::Get()->EndScene();

		Renderer::Get()->SetViewport(1280, 720, 0.0f, 1.0f);
		IFrameBuffer::Get()->BindColorBuffer();
	}

	void EditorLayer::OnGUIRender()
	{
		m_dockspacePanel.OnImGuiRender();
		m_sceneViewPanel.OnImGuiRender();
		m_hierarchyPanel.OnImGuiRender();
		m_inspectorPanel.OnImGuiRender();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_camera.OnEvent(e);
	}
}