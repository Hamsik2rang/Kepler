#pragma once

#include "Core/Base.h"
#include "Renderer/RenderState.h"

namespace kepler {

class VulkanState : public IRenderState
{
public:
	virtual ~VulkanState();

	virtual void SetShaderState(ShaderStateDescription& desc);

};

}