#pragma once

#include "Kepler.h"


namespace kepler {

	class IPanel
	{
	protected:
		std::shared_ptr<Scene> m_pContext;
		UUID m_id;

	public:
		virtual void OnImGuiRender() = 0;
		virtual void SetContext(std::shared_ptr<Scene> pScene) = 0;
		virtual std::shared_ptr<Scene> GetContext() const = 0;
		virtual UUID GetID() const = 0;
	};
}