#pragma once

#ifndef KEPLER_PLATFORM_WINDOWS
	#error Kepler Engine only support Windows.
#endif

// BIT(x) = 2^x
#define BIT_INT8(x)		(static_cast<int8_t>(1) << (x))
#define BIT_UINT8(x)	(static_cast<uint8_t>(1) << (x))
#define BIT_INT16(x)	(static_cast<int16_t>(1) << (x))
#define BIT_UINT16(x)	(static_cast<uint16_t>(1u) << (x))
#define BIT_INT32(x)	(static_cast<int32_t>(1) << (x))
#define BIT_UINT32(x)	(static_cast<uint32_t>(1u) << (x))
#define BIT_INT64(x)	(static_cast<int64_t>(1) << (x))
#define BIT_UINT64(x)	(static_cast<uint64_t>(1u) << (x))

#define BIT(x)			BIT_INT32(x)

#include "Core/Log.h"
#include "Core/Assertion.h"

#include <memory>
#include <utility>
