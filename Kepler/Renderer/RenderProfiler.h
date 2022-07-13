#pragma once

#include <imgui.h>
#include <implot.h>

#include "Core/Base.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

namespace kepler {
	class RenderProfiler : public Layer
	{
	private:
		const RenderLog* m_profile;

	public:
		RenderProfiler();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnGUIRender() override;
	};
}