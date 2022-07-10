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
		ImGui::Text("Frame : %.3f ms (%.1f FPS)", 1000.0f/ io.Framerate, io.Framerate);
		ImGui::NewLine();
		ImGui::Text("Draw Calls Count : %d", m_profile.drawCallsCount);
		ImGui::NewLine();
		
		if (ImPlot::BeginPlot("Draw Calls Plot")) {
			ImPlot::PlotLine("Draw Calls Plot", m_pDrawCallsData.x_data.get(), m_pDrawCallsData.y_data.get(), ImPlotLineData::size);
			ImPlot::EndPlot();
		}
		ImGui::NewLine();
		ImGui::Text("Batches Count : %d", m_profile.batchesCount);
		ImGui::NewLine();
		if (ImPlot::BeginPlot("Batches Plot")) {
			ImPlot::PlotLine("Batches Plot", m_pBatchesData.x_data.get(), m_pBatchesData.y_data.get(), ImPlotLineData::size);
			ImPlot::EndPlot();
		}
		ImGui::NewLine();
		ImGui::Text("Vertex Count : %d", m_profile.vertexCount);
		ImGui::NewLine();
		if (ImPlot::BeginPlot("Vertex Plot")) {
			ImPlot::PlotLine("Vertex Plot", m_pVertexData.x_data.get(), m_pVertexData.y_data.get(), ImPlotLineData::size);
			ImPlot::EndPlot();
		}
		ImGui::NewLine();
		ImGui::Text("Triangles Count : %d", m_profile.trianglesCount);
		ImGui::NewLine();
		if (ImPlot::BeginPlot("Triangles Plot")) {
			ImPlot::PlotLine("Triangles Plot", m_pTrianglesData.x_data.get(), m_pTrianglesData.y_data.get(), ImPlotLineData::size);
			ImPlot::EndPlot();
		}
		
		ImGui::NewLine();
		ImGui::End();
		ImGui::PopFont();
	}

	void RenderProfiler::OnUpdate(float deltaTime)
	{
		for (int i = 1; i < ImPlotLineData::size; i++)
		{
			m_pDrawCallsData.x_data[i - 1] = (float)(i - 1);
			m_pBatchesData	.x_data[i - 1] = (float)(i - 1);
			m_pVertexData	.x_data[i - 1] = (float)(i - 1);
			m_pTrianglesData.x_data[i - 1] = (float)(i - 1);

			m_pDrawCallsData.y_data[i - 1] = m_pDrawCallsData.y_data[i];
			m_pBatchesData	.y_data[i - 1] = m_pBatchesData.y_data	[i];
			m_pVertexData	.y_data[i - 1] = m_pVertexData.y_data	[i];
			m_pTrianglesData.y_data[i - 1] = m_pTrianglesData.y_data[i];
		}

		m_pDrawCallsData.y_data[ImPlotLineData::size - 1] = m_profile.drawCallsCount;
		m_pBatchesData	.y_data[ImPlotLineData::size - 1] = m_profile.batchesCount;
		m_pVertexData	.y_data[ImPlotLineData::size - 1] = m_profile.vertexCount;
		m_pTrianglesData.y_data[ImPlotLineData::size - 1] = m_profile.trianglesCount;
	}
}
