#pragma once

#ifndef KEPLER_PLATFORM_WINDOWS
	#error Kepler Engine only support Windows.
#endif

using int8 = int8_t;
using uint8 = uint8_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;


// BIT(x) = 2^x
#define BIT_INT8(x)		(static_cast<int8>(1) << (x))
#define BIT_UINT8(x)	(static_cast<uint8>(1) << (x))
#define BIT_INT16(x)	(static_cast<int16>(1) << (x))
#define BIT_UINT16(x)	(static_cast<uint16>(1u) << (x))
#define BIT_INT32(x)	(static_cast<int32>(1) << (x))
#define BIT_UINT32(x)	(static_cast<uint32>(1u) << (x))
#define BIT_INT64(x)	(static_cast<int64>(1) << (x))
#define BIT_UINT64(x)	(static_cast<uint64>(1u) << (x))

#define BIT(x)			BIT_INT32(x)

#include "Core/Log.h"
#include "Core/Assertion.h"

#include <memory>
#include <utility>
