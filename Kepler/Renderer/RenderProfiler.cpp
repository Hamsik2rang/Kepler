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
		ImGui::Text("Frame : %.1fFPS(%.3f ms)", io.Framerate, 1000.0f/ io.Framerate);
		ImGui::Text("Draw Call: %d", (int)m_profile->drawCallsCount.GetBackValue());
		ImGui::Text("Vertex: %d", (int)m_profile->vertexCount.GetBackValue());
		ImGui::Text("Triangles: %d", (int)m_profile->trianglesCount.GetBackValue());
		
		// TODO: 현재 Plot의 최대 출력 크기가 1000으로 고정되어 있어서 가독성이 좋지 못함
		// 추후 개선될 수 있을 경우 개선
		//if (ImPlot::BeginPlot("Plot"))
		//{
		//	ImPlot::SetupAxes("Time [s]", "Signal");
		//	ImPlot::SetupAxesLimits(0, ImPlotLineData::s_size, 0, 100);
		//	ImPlot::PlotLine("Draw Calls Plot", m_profile->drawCallsCount.GetKey(), m_profile->drawCallsCount.GetValue(), m_profile->drawCallsCount.GetCurSize());
		//	ImPlot::PlotLine("Vertex Plot", m_profile->vertexCount.GetKey(), m_profile->vertexCount.GetValue(), m_profile->vertexCount.GetCurSize());
		//	ImPlot::PlotLine("Triangles Plot", m_profile->trianglesCount.GetKey(), m_profile->trianglesCount.GetValue(), m_profile->trianglesCount.GetCurSize());
		//	ImPlot::EndPlot();
		//}
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
