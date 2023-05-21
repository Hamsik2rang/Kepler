#pragma once

#include "Kepler.h"


namespace kepler {

class IPanel
{
public:
	IPanel(Scene* pContext)
		: m_pContext{ pContext }
		, m_id{ UUID() }
	{}

	virtual void OnImGuiRender() = 0;
	virtual void SetContext(Scene* pScene) = 0;
	virtual const Scene* GetContext() const = 0;
	virtual UUID GetID() const = 0;

protected:
	Scene* m_pContext;
	UUID m_id;

};
}