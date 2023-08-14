#pragma once


#include "Core/Base.h"
#include "RenderPass.h"

namespace kepler {

enum class eRenderPassOrder : uint16_t
{
	BEFORE_RENDERING = 0,

	OPAQUE_PASS = 450,

	TRANSPARENT_PASS = 550,

	DEBUG_PASS = 700,

	AFTER_RENDERING = 1000,
};

struct RenderPathSpecification
{
	bool bIsRenderOpaquePass;
	bool bIsRenderTransparentPass;
	bool bIsRenderDebugPass;

	// TODO: 추후 커스텀 렌더패스 지원하기
	//struct CustomPassSpecification
	//{
	//	std::string passName;
	//	bool bIsRender;
	//	eRenderPassOrder order;
	//	uint16_t orderOffset;
	//};
	//CustomPassSpecification customPassSpec[3];
};

class IRenderPath
{
public:
	IRenderPath();
	virtual ~IRenderPath();
protected:
	RenderPathSpecification _pathSpec;
	std::list<IRenderPass*> _renderPasses;
};

class ForwardRenderPath
{
public:


private:
};

class DeferredRenderPath
{
public:

private:
};

}