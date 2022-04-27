#pragma once
#include <cassert>

#ifdef KEPLER_ASSERTION_ENABLE
	#define KEPLER_ASSERT(x, ...) { if (!(x)) { KEPLER_ERROR("Assertion Failed: {0} in {1}(line: {2})", __VA_ARGS__, __FILE__, __LINE__); __debugbreak(); } }	
	#define KEPLER_CORE_ASSERT(x, ...) { if(!(x)) { KEPLER_CORE_ERROR("Assertion Failed: {0} in {1}(line: {2})", __VA_ARGS__, __FILE__, __LINE__); __debugbreak(); } }
	#define KEPLER_NEVER_HAPPEN assert(0);

#else
	#define KEPLER_ASSERT(x, ...)
	#define KEPLER_CORE_ASSERT(x, ...)
	#define KEPLER_NEVER_HAPPEN __assume(0);

#endif
