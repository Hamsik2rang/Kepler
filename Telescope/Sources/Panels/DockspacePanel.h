#pragma once

#include "Panel.h"

namespace kepler {

class DockspacePanel : public IPanel
{
public:
	DockspacePanel(Scene* pContext)
		: m_bPadding{ false }
		, m_bDockspaceOpen{ true }
		, m_bDockspaceFull{ true }
		, m_dockspaceFlags{ ImGuiDockNodeFlags_None }
		, IPanel(pContext)
	{}

	virtual void OnImGuiRender() override;
	inline virtual void SetContext(Scene* pScene) override { m_pContext = pScene; }
	inline virtual const Scene* GetContext() const override { return m_pContext; }
	inline virtual UUID GetID() const override { return m_id; }

private:
	bool m_bPadding;
	bool m_bDockspaceOpen;
	bool m_bDockspaceFull;
	ImGuiDockNodeFlags m_dockspaceFlags;

};
}