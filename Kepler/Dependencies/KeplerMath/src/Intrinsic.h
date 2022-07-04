#pragma once

#include <intrin.h>

#define KP_MM_SHUFFLE_LE(fp0, fp1, fp2, fp3) _MM_SHUFFLE((fp3), (fp2), (fp1), (fp0))
