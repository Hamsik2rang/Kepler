#include "kepch.h"
#include "RenderProfiler.h"

#include <imgui.h>


namespace kepler {

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

		ImGui::Begin("Debug Mode");
		ImGui::Text("Frame : %.3f ms (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::NewLine();
		ImGui::NewLine();

		ImGui::End();
		ImGui::PopFont();
	}


}
