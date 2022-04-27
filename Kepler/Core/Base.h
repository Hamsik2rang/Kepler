#pragma once

#ifndef KEPLER_PLATFORM_WINDOWS
	#error Kepler Engine only support Windows.
#endif




// BIT(x) = 2^x
#define BIT(x) (static_cast<uint64_t>(1) << (x))

#include "Core/Log.h"
#include "Core/Assertion.h"