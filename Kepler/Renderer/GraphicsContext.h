#pragma once

#include "Core/Base.h"

namespace kepler {

	class IGraphicsAPIContext
	{
	public:
		virtual ~IGraphicsAPIContext() {}
		virtual void Init() = 0;
		// swap buffer
		virtual void Present() = 0;
	};
}