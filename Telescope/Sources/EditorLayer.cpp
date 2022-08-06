#include "EditorLayer.h"

void EditorLayer::OnAttach()
{
	kepler::IFrameBuffer::Get()->AddGBuffer(1, 1);

}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnUpdate(const float deltaTime)
{

}

void EditorLayer::OnRender()
{

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
}

void EditorLayer::OnEvent(kepler::Event& e)
{
	m_camera.OnEvent(e);
}