#pragma once

#include "Panel.h"

namespace kepler {

	class InspectorPanel : public IPanel
	{
	public:
		virtual void OnImGuiRender() override;
		inline virtual void SetContext(std::shared_ptr<Scene> pScene) override { m_pContext = pScene; }
		inline virtual std::shared_ptr<Scene> GetContext() const override { return m_pContext; }
		inline virtual UUID GetID() const override { return m_id; }


	};
}