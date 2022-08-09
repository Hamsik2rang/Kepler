#include "EditorLayer.h"
#include <d3d11.h>

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

	std::shared_ptr<kepler::IVertexBuffer> pVB = kepler::IVertexBuffer::Create(vertices, sizeof(vertices), kepler::eBufferUsage::Default);
	pVB->SetLayout({
		{"POSITION", 0, kepler::eShaderDataType::Float3, 0, sizeof(float) * 3},
		{"COLOR", 0, kepler::eShaderDataType::Float4, sizeof(float) * 3, sizeof(float) * 4}
		});

	std::shared_ptr<kepler::IIndexBuffer> pIB = kepler::IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), kepler::eBufferUsage::Default);

	m_pCubeVA = kepler::IVertexArray::Create();
	m_pCubeVA->AddVertexBuffer(pVB);
	m_pCubeVA->SetIndexBuffer(pIB);
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnUpdate(const float deltaTime)
{
	m_time += deltaTime;
	kepler::IRenderState::Get()->Bind();
	if (m_viewportWidth != m_lastViewportWidth || m_viewportHeight != m_lastViewportHeight)
	{
		// 1. FrameBuffer(Scene RenderTarget) Resizing and clear it's background.
		kepler::IFrameBuffer::Get()->ResizeGBuffer(0, 1, m_viewportWidth, m_viewportHeight);
		float col[]{ 0.1f, 0.1f, 0.1f, 1.0f };
		kepler::IFrameBuffer::Get()->ClearGBuffer(0, 1, col);
		// 2. Set Aspect
		m_camera.SetAspect((float)m_viewportWidth / (float)m_viewportHeight);
		
		m_lastViewportWidth = m_viewportWidth;
		m_lastViewportHeight = m_viewportHeight;
	}

	kepler::IFrameBuffer::Get()->BindGBuffer(0, 1);

	m_camera.OnUpdate(deltaTime);
}

void EditorLayer::OnRender()
{
	kepler::Renderer::Get()->SetViewport(m_viewportWidth, m_viewportHeight, 0.0f, 1.0f);
	kepler::Renderer::Get()->BeginScene(m_camera);

	kepler::Renderer::Get()->Submit(m_pCubeVA, kepler::math::GetRotationMatrixX(m_time) * kepler::math::GetRotationMatrixY(m_time / 3.0f) * kepler::math::GetTranslateMatrix({ 0.0f, 0.0f, 4.0f }));
	kepler::Renderer::Get()->Submit(m_pCubeVA, kepler::math::GetScalingMatrix({ 2.5f, 2.5f, 2.5f }) * kepler::math::GetRotationMatrixX(m_time / 2.0f) * kepler::math::GetRotationMatrixZ(m_time / 1.8f) * kepler::math::GetTranslateMatrix({ 0.0f, 0.0f, 60.0f }));

	kepler::Renderer::Get()->EndScene();

	kepler::Renderer::Get()->SetViewport(1280, 720, 0.0f, 1.0f);
	kepler::IFrameBuffer::Get()->BindColorBuffer();
}

void EditorLayer::OnGUIRender()
{
	static bool bDockspaceOpen = true;
	static bool bFullDockspace = true;
	static bool bPadding = false;
	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (bFullDockspace)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
	{
		windowFlags |= ImGuiWindowFlags_NoBackground;
	}

	// NOTE: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!bPadding)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	}
	ImGui::Begin("Telescope DockSpace Demo", &bDockspaceOpen, windowFlags);
	if (!bPadding)
	{
		ImGui::PopStyleVar();
	}

	if (bFullDockspace)
	{
		ImGui::PopStyleVar(2);
	}

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Telescope Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Open", nullptr, nullptr);
			if (ImGui::MenuItem("Exit", nullptr))
			{
				kepler::Application::Get()->Shutdown();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", nullptr, &bFullDockspace);
			ImGui::MenuItem("Padding", nullptr, &bPadding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspaceFlags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspaceFlags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspaceFlags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspaceFlags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspaceFlags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspaceFlags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspaceFlags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspaceFlags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, bFullDockspace)) { dockspaceFlags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			//ImGui::Separator();

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse);

	// Scene View Begin
	ImGui::SetScrollY(0);
	ID3D11ShaderResourceView* buffer = (ID3D11ShaderResourceView*)kepler::IFrameBuffer::Get()->GetBuffer(kepler::eFrameBufferType::Color, 0);
	auto size = ImGui::GetWindowSize();
	m_bIsSceneFocuced = ImGui::IsWindowFocused();
	m_viewportWidth = size.x;
	m_viewportHeight = size.y;
	ImGui::Image(buffer, size, { 0, 1 }, { 1, 0 });

	ImGui::End();
	ImGui::PopStyleVar();
	// Scene View End

	// Hierarchy View Begin
	ImGui::Begin("Hierarchy");

	ImGui::Text("Cube");
	ImGui::Text("Cube(1)");

	ImGui::End();
	// Hierarchy View End

	// Inspector View Begin
	ImGui::Begin("Inspector");

	ImGui::Text("There will be an Inspector Panel");

	ImGui::End();
	// Inspector View End
}

void EditorLayer::OnEvent(kepler::Event& e)
{
	m_camera.OnEvent(e);
}