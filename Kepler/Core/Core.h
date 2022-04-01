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