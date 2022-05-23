#pragma once

#include <cmath>

#include "Intrinsic.h"
#include "Constant.h"
#include "Vector4.hpp"

namespace kepler {
	namespace math {

		const Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t)
		{
			t = math::Clamp(t, 0.0f, 1.0f);
			Vector3 vt{};
			vt = v0 * (1.0f - t) + v1 * t;

			return vt;
		}

		const Vector4 Lerp(const Vector4& v0, const Vector4& v1, float t)
		{
			t = math::Clamp(t, 0.0f, 1.0f);
			Vector4 vt{};
			vt.ps = _mm_add_ps(_mm_mul_ps(v0.ps, _mm_set_ps1(1.0f - t)), _mm_mul_ps(v1.ps, _mm_set_ps1(t)));

			return vt;
		}

		const Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t)
		{
			t = math::Clamp(t, 0.0f, 1.0f);

			auto qa = q0 / q0.Length();
			auto qb = q1 / q1.Length();

			float dot = Dot(qa.v, qb.v) + qa.w * qb.w;
			float theta = ::acosf(dot);
			float sin = ::sinf(theta);

			float ta = ::sinf(theta * (1.0f - t)) / sin;
			float tb = ::sinf(theta * t) / sin;

			Quaternion qt = ta * qa + tb * qb;

			return qt;
		}
	}
}