#include "SceneViewPanel.h"

#include <d3d11.h>


namespace kepler {

	void SceneViewPanel::OnImGuiRender()
	{
		// Padding, Scrolling 제거를 위한 코드
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse);

		// Scene View Begin
		ImGui::SetScrollY(0);
		ID3D11ShaderResourceView* buffer = (ID3D11ShaderResourceView*)IFrameBuffer::Get()->GetBuffer(eFrameBufferType::Color, 0);
		auto size = ImGui::GetWindowSize();
		m_bIsSceneFocuced = ImGui::IsWindowFocused();
		m_lastViewportWidth = m_viewportWidth;
		m_lastViewportHeight = m_viewportHeight;
		m_viewportWidth = size.x;
		m_viewportHeight = size.y;
		ImGui::Image(buffer, size, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
		// Scene View End
	}
}