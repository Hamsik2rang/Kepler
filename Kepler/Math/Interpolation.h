#pragma once

#include <cmath>

#include "Intrinsic.h"
#include "Constant.h"
#include "Vector.h"
#include "Quaternion.h"

namespace kepler {
	namespace math {

		const Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t);
		const Vector4 Lerp(const Vector4& v0, const Vector4& v1, float t);
		const Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);
	
	}
}