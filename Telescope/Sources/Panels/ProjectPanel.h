#pragma once

#include "Panel.h"

namespace kepler {

class ProjectPanel : public IPanel
{
public:
	ProjectPanel(Scene* pContext)
		: IPanel(pContext)
	{}

	virtual void OnImGuiRender() override;
	inline virtual void SetContext(Scene* pScene) override { m_pContext = pScene; }
	inline virtual const Scene* GetContext() const override { return m_pContext; }
	inline virtual UUID GetID() const override { return m_id; }
};
}