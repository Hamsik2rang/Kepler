#include "kepch.h"
#include "RenderPass.h"


namespace kepler {

IRenderPass::IRenderPass()
{}

IRenderPass::~IRenderPass()
{

}

IRenderPass* IRenderPass::Create(eRenderPassType type)
{
	switch (type)
	{
	case eRenderPassType::FORWARD:
	{
		return new ForwardRenderPass();
	}
	case eRenderPassType::DEFERRED:
	default:
	{
		KEPLER_ASSERT(false, "This type of RenderPass is not supported yet!");
		return nullptr;
	}
	}

	return nullptr;
}

ForwardRenderPass::ForwardRenderPass()
{

}

ForwardRenderPass::~ForwardRenderPass()
{

}

DeferredRenderPass::DeferredRenderPass()
{

}

DeferredRenderPass::~DeferredRenderPass()
{

}

}