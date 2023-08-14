#pragma once

#include "Core/Base.h"



namespace kepler {

class Renderer;

enum class eRenderPassType
{
	NONE = 0,
	FORWARD,
	DEFERRED,
	CUSTOM,
};

class IRenderPass
{
public:
	friend Renderer;

	IRenderPass();
	~IRenderPass();


private:

	static IRenderPass* Create(eRenderPassType type);
};

class ForwardRenderPass : public IRenderPass
{
public:
	ForwardRenderPass();
	~ForwardRenderPass();

private:
};

class DeferredRenderPass : public IRenderPass
{
public:
	DeferredRenderPass();
	~DeferredRenderPass();

private:
};

}