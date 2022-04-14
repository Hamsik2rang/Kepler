#pragma once

#ifdef KEPLER_PLATFORM_WINDOWS
	#ifdef KEPLER_BUILD_DLL
		#define KEPLER_API __declspec(dllexport)
	#else
		#define KEPLER_API __declspec(dllimport)
	#endif
#else
	#error Kepler Engine only support Windows.
#endif

#ifdef KEPLER_ASSERTION_ENABLE
	#define KEPLER_ASSERT(x, ...) { if (!(x)) { KEPLER_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define KEPLER_CORE_ASSERT(x, ...) { if(!(x)) { KEPLER_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define KEPLER_ASSERT(x, ...)
	#define KEPLER_CORE_ASSERT(x, ...)
#endif


// BIT(x) = 2^x
#define BIT(x) (1 << (x))

#include "Core/Log.h"