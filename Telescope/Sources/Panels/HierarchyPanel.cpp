#include "HierarchyPanel.h"

namespace kepler {

	void HierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		//...
		m_pContext->Each<TagComponent>([](Entity* pEntity)->void
			{
				TagComponent* tc = pEntity->GetComponent<TagComponent>();
				ImGui::Text(tc->name.c_str());
			});

		ImGui::End();
	}
}