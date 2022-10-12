#include "HierarchyPanel.h"

namespace kepler {

	void HierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		//...

		ImGui::Text("Cube");
		ImGui::Text("Cube(1)");

		ImGui::End();
	}
}