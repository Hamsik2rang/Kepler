#include "InspectorPanel.h"

namespace kepler {

void InspectorPanel::OnImGuiRender()
{
	ImGui::Begin("Inspector");

	ImGui::Text("There will be an Inspector Panel");

	ImGui::End();
}
}