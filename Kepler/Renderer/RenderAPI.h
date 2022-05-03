#pragma once

#include "Core/Base.h"

namespace kepler {

	enum class eAPI
	{
		None = 0,
		DirectX11 = 1,
	};

	class IRenderAPI
	{
	public:
		virtual ~IRenderAPI() {}
		virtual bool Init() = 0;
		// swap buffer
		virtual void SwapBuffer() = 0;
	};
}