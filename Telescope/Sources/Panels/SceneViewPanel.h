#pragma once

#include "Panel.h"

namespace kepler {

	class SceneViewPanel : public IPanel
	{
	private:
		bool m_bIsSceneFocuced;
		float m_viewportWidth;
		float m_viewportHeight;
		float m_lastViewportWidth;
		float m_lastViewportHeight;


	public:
		SceneViewPanel(Scene* pContext)
			: m_bIsSceneFocuced{ false }
			, m_viewportWidth{ 0.0f }
			, m_viewportHeight{ 0.0f }
			, m_lastViewportWidth{ 0.0f }
			, m_lastViewportHeight{ 0.0f }
			, IPanel(pContext)
		{}

		virtual void OnImGuiRender() override;
		inline virtual void SetContext(Scene* pScene) override { m_pContext = pScene; }
		inline virtual const Scene* GetContext() const override { return m_pContext; }
		inline virtual UUID GetID() const override { return m_id; }

		inline Vec2f GetViewportSize() { return Vec2f{ m_viewportWidth, m_viewportHeight }; }
		inline bool IsViewportResized() { return (uint32_t)m_viewportHeight != (uint32_t)m_lastViewportHeight || (uint32_t)m_viewportWidth != (uint32_t)m_lastViewportWidth; }
	};
}