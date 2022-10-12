#pragma once


#include <Kepler.h>
#include "Panels/DockspacePanel.h"
#include "Panels/HierarchyPanel.h"
#include "Panels/SceneViewPanel.h"
#include "Panels/ProjectPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/GeneralPanel.h"

namespace kepler {

	class EditorLayer : public Layer
	{
	private:
		EditorCamera m_camera;

		Entity* m_pCubeEntity;
		Scene m_scene;

		float m_time;
		bool m_bIsSceneFocuced;

		DockspacePanel m_dockspacePanel;
		SceneViewPanel m_sceneViewPanel;
		HierarchyPanel m_hierarchyPanel;
		InspectorPanel m_inspectorPanel;

	public:

		EditorLayer()
			: Layer("Editor")
			, m_camera{ EditorCamera{ Vec3f{ 0.0f, 1.0f, -6.0f }, Vec3f{ 0.0f, 1.0f, 0.0f }, 10.0f, 45.0f, 16.0f / 9.0f, 0.1f, 1000.0f } }
			, m_time{ 0.0f }
			, m_bIsSceneFocuced{ false }
			, m_pCubeEntity{ nullptr }
			, m_dockspacePanel{ &m_scene }
			, m_sceneViewPanel{ &m_scene }
			, m_hierarchyPanel{ &m_scene }
			, m_inspectorPanel{ &m_scene }
		{}

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnGUIRender() override;
		virtual void OnEvent(Event& e) override;
	};
}