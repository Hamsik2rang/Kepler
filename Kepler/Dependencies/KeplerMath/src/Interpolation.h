#pragma once

#include <cmath>
#include <algorithm>

#include "Intrinsic.h"
#include "Constant.h"
#include "Vector.h"
#include "Quaternion.hpp"

namespace kepler {
	namespace math {
		//! 두 실수를 선형 보간합니다.
		//! \author Im Yongsik
		//! \param f0: 보간할 실수 0. t가 0.0일 경우의 보간 결과입니다.
		//! \param f1: 보간할 실수 1. t가 1.0일 경우의 보간 결과입니다.
		//! \param t: 보간 계수. [0.0, 1.0]의 범위로 강제됩니다.
		//! \return t에 따라 선형 보간된 f0과 f1사이의 실수
		const float KEPLER_MATH_API Lerp(const float f0, const float f1, float t);

		//! 두 2차원 벡터를 선형 보간합니다.
		//! \author Im Yongsik
		//! \param v0: 보간할 벡터 0. t가 0.0일 경우의 보간 결과입니다.
		//! \param v1: 보간할 벡터 1. t가 1.0일 경우의 보간 결과입니다.
		//! \param t: 보간 계수. [0.0, 1.0]의 범위로 강제됩니다.
		//! \return t에 따라 선형 보간된 v0과 v1사이의 2차원 벡터
		const Vector2 KEPLER_MATH_API Lerp(const Vector2& v0, const Vector2& v1, float t);

		//! 두 3차원 벡터를 선형 보간합니다.
		//! \author Im Yongsik
		//! \param v0: 보간할 벡터 0. t가 0.0일 경우의 보간 결과입니다.
		//! \param v1: 보간할 벡터 1. t가 1.0일 경우의 보간 결과입니다.
		//! \param t: 보간 계수. [0.0, 1.0]의 범위로 강제됩니다.
		//! \return t에 따라 선형 보간된 v0과 v1사이의 3차원 벡터
		const Vector3 KEPLER_MATH_API Lerp(const Vector3& v0, const Vector3& v1, float t);

		//! 두 4차원 벡터를 선형 보간합니다.
		//! \author Im Yongsik
		//! \param v0: 보간할 벡터 0. t가 0.0일 경우의 보간 결과입니다.
		//! \param v1: 보간할 벡터 1. t가 1.0일 경우의 보간 결과입니다.
		//! \param t: 보간 계수. [0.0, 1.0]의 범위로 강제됩니다.
		//! \return t에 따라 선형 보간된 v0과 v1사이의 4차원 벡터
		const Vector4 KEPLER_MATH_API Lerp(const Vector4& v0, const Vector4& v1, float t);

		//! 두 쿼터니언을 구면 선형 보간합니다.
		//! \author Im Yongsik
		//! \param q0: 보간할 쿼터니언 0. t가 0.0일 경우의 보간 결과입니다.
		//! \param q1: 보간할 쿼터니언 1. t가 1.0일 경우의 보간 결과입니다.
		//! \param t: 보간 계수. [0.0, 1.0]의 범위로 강제됩니다.
		//! \return t에 따라 구면 선형 보간된 q0과 q1사이의 쿼터니언
		const Quaternion KEPLER_MATH_API Slerp(const Quaternion& q0, const Quaternion& q1, float t);
	
	}
}