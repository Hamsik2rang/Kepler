#pragma once

#include <memory>

#include "Core/Base.h"

namespace kepler {

	class IRenderState
	{
	private:
		static std::shared_ptr<IRenderState> s_pInstance;

	public:
		

		static std::shared_ptr<IRenderState> Create();
	};

}