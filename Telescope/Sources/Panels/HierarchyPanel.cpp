#include "HierarchyPanel.h"

namespace kepler {

void HierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Hierarchy");

	m_pContext->Each([](Entity* pEntity)->void
		{
			if (pEntity->HasComponent<TagComponent>())
			{
				auto tc = pEntity->GetComponent<TagComponent>();
				if (pEntity->GetChildrenEntity().empty())
				{
					ImGui::TreeNodeEx((void*)0, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, tc->name.c_str());
				}
				else
				{
					ImGui::TreeNodeEx((void*)0, 0, tc->name.c_str());
				}
			}
		});

	ImGui::End();
}
}