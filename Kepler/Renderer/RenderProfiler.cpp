#include "kepch.h"
#include "RenderProfiler.h"

namespace kepler {

	RenderProfiler* RenderProfiler::s_pInstance = nullptr;

	void RenderProfiler::Create()
	{
		if (s_pInstance)
		{
			return;
		}
		s_pInstance = new RenderProfiler;
	}

	RenderProfiler* RenderProfiler::Get()
	{
		if (!s_pInstance)
		{
			s_pInstance = new RenderProfiler;
		}
		return s_pInstance;
	}

	void RenderProfiler::OnAttach()
	{
		// GUI용 폰트 초기화
		ImGuiIO& io = ImGui::GetIO();
		m_pFont = io.Fonts->AddFontFromFileTTF("./Assets/OpenSans-Regular.ttf", 18.0f);
	}

	void RenderProfiler::OnGUIRender()
	{
		ImGui::PushFont(m_pFont);
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Begin("Render Profiler");
		ImGui::Text("Frame : %.3f ms (%.1f FPS)", 1000.0f / m_profile.frameRate, m_profile.frameRate);
		ImGui::NewLine();
		ImGui::Text("Draw Calls Count : %d", m_profile.drawCallsCount);
		ImGui::NewLine();
		ImGui::Text("Batches Count : %d", m_profile.batchesCount);
		ImGui::NewLine();
		ImGui::Text("Vertex Count : %d", m_profile.vertexCount);
		ImGui::NewLine();
		ImGui::Text("Triangles Count : %d", m_profile.trianglesCount);
		ImGui::NewLine();
		ImGui::End();
		ImGui::PopFont();
	}

	void RenderProfiler::OnUpdate(float deltaTime)
	{
		m_profile.frameRate = ImGui::GetIO().Framerate;
	}
}
