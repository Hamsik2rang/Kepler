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
		ImGui::PlotLines("Draw Calls", m_pDrawCallsCounts.get(), CHART_SIZE);
		ImGui::NewLine();
		ImGui::Text("Batches Count : %d", m_profile.batchesCount);
		ImGui::NewLine();
		ImGui::PlotLines("Batches", m_pDrawCallsCounts.get(), CHART_SIZE);
		ImGui::NewLine();
		ImGui::Text("Vertex Count : %d", m_profile.vertexCount);
		ImGui::NewLine();
		ImGui::PlotLines("Vertex", m_pDrawCallsCounts.get(), CHART_SIZE);
		ImGui::NewLine();
		ImGui::Text("Triangles Count : %d", m_profile.trianglesCount);
		ImGui::NewLine();
		ImGui::PlotLines("Triangles", m_pDrawCallsCounts.get(), CHART_SIZE);
		ImGui::NewLine();
		ImGui::End();
		ImGui::PopFont();
	}

	void RenderProfiler::OnUpdate(float deltaTime)
	{
		m_profile.frameRate = ImGui::GetIO().Framerate;

		for (int i = 1; i < CHART_SIZE; i++)
		{
			m_pDrawCallsCounts	[i - 1] = m_pDrawCallsCounts[i];
			m_pBatchesCounts	[i - 1] = m_pBatchesCounts	[i];
			m_pVertexCounts		[i - 1] = m_pVertexCounts	[i];
			m_pTrianglesCounts	[i - 1] = m_pTrianglesCounts[i];
		}

		m_pDrawCallsCounts	[CHART_SIZE - 1] = m_profile.drawCallsCount;
		m_pBatchesCounts	[CHART_SIZE - 1] = m_profile.batchesCount;
		m_pVertexCounts		[CHART_SIZE - 1] = m_profile.vertexCount;
		m_pTrianglesCounts	[CHART_SIZE - 1] = m_profile.trianglesCount;
	}
}
