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
		// GUI�� ��Ʈ �ʱ�ȭ
		ImGuiIO& io = ImGui::GetIO();
		m_pFont = io.Fonts->AddFontFromFileTTF("./Assets/OpenSans-Regular.ttf", 18.0f);
	}

	void RenderProfiler::OnGUIRender()
	{
		ImGui::PushFont(m_pFont);
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Begin("Render Profiler");
		ImGui::Text("Frame : %.3f ms (%.1f FPS)", 1000.0f/ io.Framerate, io.Framerate);
		ImGui::Text("Draw Calls Count : %d", m_profile.drawCallsCount);
		ImGui::Text("Batches Count : %d", m_profile.batchesCount);
		ImGui::Text("Vertex Count : %d", m_profile.vertexCount);
		ImGui::Text("Triangles Count : %d", m_profile.trianglesCount);
		if (ImPlot::BeginPlot("Plot")) {
			ImPlot::SetupAxes("Time [s]", "Signal");
			ImPlot::SetupAxesLimits(0, ImPlotLineData::size, 0, 100);
			ImPlot::PlotLine("Draw Calls Plot", m_drawCallsData.x_data.get(), m_drawCallsData.y_data.get(), ImPlotLineData::size);
			ImPlot::PlotLine("Batches Plot", m_batchesData.x_data.get(), m_batchesData.y_data.get(), ImPlotLineData::size);
			ImPlot::PlotLine("Vertex Plot", m_vertexData.x_data.get(), m_vertexData.y_data.get(), ImPlotLineData::size);
			ImPlot::PlotLine("Triangles Plot", m_trianglesData.x_data.get(), m_trianglesData.y_data.get(), ImPlotLineData::size);
			ImPlot::EndPlot();
		}
		ImGui::End();
		ImGui::PopFont();
	}

	void RenderProfiler::OnUpdate(float deltaTime)
	{
		for (int i = 1; i < ImPlotLineData::size; i++)
		{
			m_drawCallsData	.x_data[i - 1] = (float)(i - 1);
			m_batchesData	.x_data[i - 1] = (float)(i - 1);
			m_vertexData	.x_data[i - 1] = (float)(i - 1);
			m_trianglesData	.x_data[i - 1] = (float)(i - 1);

			m_drawCallsData	.y_data[i - 1] = m_drawCallsData.y_data	[i];
			m_batchesData	.y_data[i - 1] = m_batchesData.y_data	[i];
			m_vertexData	.y_data[i - 1] = m_vertexData.y_data	[i];
			m_trianglesData	.y_data[i - 1] = m_trianglesData.y_data	[i];
		}

		m_drawCallsData	.y_data[ImPlotLineData::size - 1] = m_profile.drawCallsCount;
		m_batchesData	.y_data[ImPlotLineData::size - 1] = m_profile.batchesCount;
		m_vertexData	.y_data[ImPlotLineData::size - 1] = m_profile.vertexCount;
		m_trianglesData	.y_data[ImPlotLineData::size - 1] = m_profile.trianglesCount;
	}
}