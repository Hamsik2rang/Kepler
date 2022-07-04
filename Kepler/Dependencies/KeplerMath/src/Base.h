#pragma once

#ifdef KEPLER_MATH_BUILD_DLL
	#define KEPLER_MATH_API __declspec(dllexport)
#else
	#define KEPLER_MATH_API __declspec(dllimport)
#endif
