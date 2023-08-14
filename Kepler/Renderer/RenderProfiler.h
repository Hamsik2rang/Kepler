#pragma once

#include <imgui.h>

#include "Core/Base.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

namespace kepler {
class RenderProfiler : public Layer
{
public:
	RenderProfiler();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnGUIRender() override;

private:
	const RenderLog* m_profile;

};
}