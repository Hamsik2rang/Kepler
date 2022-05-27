#pragma once
#include <cassert>

#ifdef _DEBUG
	#ifndef KEPLER_ASSERTION_ENABLE
		#define KEPLER_ASSERTION_ENABLE
	#endif

#else
	#undef KEPLER_ASSERTION_ENABLE

#endif

#ifdef KEPLER_ASSERTION_ENABLE
	#define KEPLER_ASSERT(x, ...) { if (!(x)) { KEPLER_ERROR("Assertion Failed: {0} in {1}(line: {2})", __VA_ARGS__, __FILE__, __LINE__); __debugbreak(); } }	
	#define KEPLER_CORE_ASSERT(x, ...) { if(!(x)) { KEPLER_CORE_ERROR("Assertion Failed: {0} in {1}(line: {2})", __VA_ARGS__, __FILE__, __LINE__); __debugbreak(); } }
	#define KEPLER_NEVER_HAPPEN assert(0);

	#define KEPLER_ASSERT_NO_MESSAGE(x) {if(!(x)) { KEPLER_ERROR("Assertion Failed: {0}(line: {1}", __FILE__, __LINE__); __debugbreak(); } }
	#define KEPLER_CORE_ASSERT_EXCEPT_MESSAGE(x) {if(!(x)) { KEPLER_CORE_ERROR("Assertion Failed: {0}(line: {1}", __FILE__, __LINE__); __debugbreak(); } }
	
	#define KEPLER_ASSERT_WITH_ERROR_CODE(x) \
	{ \
		if(!(x)) \
		{ \
			KEPLER_ERROR("Assertion Failed: ErrorCode{0} in {1}(line: {2})", GetLastError(), __FILE__, __LINE__);\
			__debugbreak();\
		}\
	 }
	#define KEPLER_CORE_ASSERT_WITH_ERROR_CODE(x) \
	{\
		if(!(x)) \
		{\
			KEPLER_CORE_ERROR("Assertion Failed: ErrorCode{0} in {1}(line: {2})", GetLastError(), __FILE__, __LINE__);\
			__debugbreak();\
		}\
	}

#else
	#define KEPLER_ASSERT(x, ...)
	#define KEPLER_CORE_ASSERT(x, ...)
	#define KEPLER_NEVER_HAPPEN __assume(0);
	#define KEPLER_ASSERT_NO_MESSAGE(x)
	#define KEPLER_CORE_ASSERT_EXCEPT_MESSAGE(x)
	#define KEPLER_ASSERT_WITH_ERROR_CODE(x) 
	#define KEPLER_CORE_ASSERT_WITH_ERROR_CODE(x)

#endif
