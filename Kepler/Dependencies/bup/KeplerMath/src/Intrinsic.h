#pragma once

#include <intrin.h>

#define KP_MM_SHUFFLE_LE(fp0, fp1, fp2, fp3) _MM_SHUFFLE((fp3), (fp2), (fp1), (fp0))
#define KP_PERMUTE_PS(v, c) _mm_shuffle_ps((v), (v), c)
#define KP_FNMADD_PS(a, b, c) _mm_sub_ps((c), _mm_mul_ps((a), (b)))