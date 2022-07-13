#include "kepch.h"
#include "RenderProfiler.h"
#include "RenderLog.h"

namespace kepler {
	void RenderProfiler::OnGUIRender()
	{
		if (!m_profile)
		{
			return;
		}

		ImGuiIO& io = ImGui::GetIO();

		ImGui::Begin("Render Profiler");
		ImGui::Text("Frame : %.3f ms (%.1f FPS)", 1000.0f/ io.Framerate, io.Framerate);
		ImGui::Text("Draw Calls Count : %d", m_profile->drawCallsCount.GetBackValue());
		ImGui::Text("Batches Count : %d", m_profile->batchesCount.GetBackValue());
		ImGui::Text("Vertex Count : %d", m_profile->vertexCount.GetBackValue());
		ImGui::Text("Triangles Count : %d", m_profile->trianglesCount.GetBackValue());
		if (ImPlot::BeginPlot("Plot"))
		{
			ImPlot::SetupAxes("Time [s]", "Signal");
			ImPlot::SetupAxesLimits(0, ImPlotLineData::s_size, 0, 100);
			ImPlot::PlotLine("Draw Calls Plot", m_profile->drawCallsCount.GetKey(), m_profile->drawCallsCount.GetValue(), m_profile->drawCallsCount.GetCurSize());
			ImPlot::PlotLine("Batches Plot", m_profile->batchesCount.GetKey(), m_profile->batchesCount.GetValue(), m_profile->batchesCount.GetCurSize());
			ImPlot::PlotLine("Vertex Plot", m_profile->vertexCount.GetKey(), m_profile->vertexCount.GetValue(), m_profile->vertexCount.GetCurSize());
			ImPlot::PlotLine("Triangles Plot", m_profile->trianglesCount.GetKey(), m_profile->trianglesCount.GetValue(), m_profile->trianglesCount.GetCurSize());
			ImPlot::EndPlot();
		}
		ImGui::End();
	}

	RenderProfiler::RenderProfiler()
		: m_profile(nullptr)
	{
	}

	void RenderProfiler::OnUpdate(float deltaTime)
	{
		m_profile = &Renderer2D::Get()->GetRenderLog();
	}
}
