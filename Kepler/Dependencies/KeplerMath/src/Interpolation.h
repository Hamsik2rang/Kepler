#pragma once

#include <cmath>
#include <algorithm>

#include "Intrinsic.h"
#include "Constant.h"
#include "Vector.h"
#include "Quaternion.hpp"

namespace kepler {
	namespace math {
		//! �� �Ǽ��� ���� �����մϴ�.
		//! \author Im Yongsik
		//! \param f0: ������ �Ǽ� 0. t�� 0.0�� ����� ���� ����Դϴ�.
		//! \param f1: ������ �Ǽ� 1. t�� 1.0�� ����� ���� ����Դϴ�.
		//! \param t: ���� ���. [0.0, 1.0]�� ������ �����˴ϴ�.
		//! \return t�� ���� ���� ������ f0�� f1������ �Ǽ�
		const float KEPLER_MATH_API Lerp(const float f0, const float f1, float t);

		//! �� 2���� ���͸� ���� �����մϴ�.
		//! \author Im Yongsik
		//! \param v0: ������ ���� 0. t�� 0.0�� ����� ���� ����Դϴ�.
		//! \param v1: ������ ���� 1. t�� 1.0�� ����� ���� ����Դϴ�.
		//! \param t: ���� ���. [0.0, 1.0]�� ������ �����˴ϴ�.
		//! \return t�� ���� ���� ������ v0�� v1������ 2���� ����
		const Vector2 KEPLER_MATH_API Lerp(const Vector2& v0, const Vector2& v1, float t);

		//! �� 3���� ���͸� ���� �����մϴ�.
		//! \author Im Yongsik
		//! \param v0: ������ ���� 0. t�� 0.0�� ����� ���� ����Դϴ�.
		//! \param v1: ������ ���� 1. t�� 1.0�� ����� ���� ����Դϴ�.
		//! \param t: ���� ���. [0.0, 1.0]�� ������ �����˴ϴ�.
		//! \return t�� ���� ���� ������ v0�� v1������ 3���� ����
		const Vector3 KEPLER_MATH_API Lerp(const Vector3& v0, const Vector3& v1, float t);

		//! �� 4���� ���͸� ���� �����մϴ�.
		//! \author Im Yongsik
		//! \param v0: ������ ���� 0. t�� 0.0�� ����� ���� ����Դϴ�.
		//! \param v1: ������ ���� 1. t�� 1.0�� ����� ���� ����Դϴ�.
		//! \param t: ���� ���. [0.0, 1.0]�� ������ �����˴ϴ�.
		//! \return t�� ���� ���� ������ v0�� v1������ 4���� ����
		const Vector4 KEPLER_MATH_API Lerp(const Vector4& v0, const Vector4& v1, float t);

		//! �� ���ʹϾ��� ���� ���� �����մϴ�.
		//! \author Im Yongsik
		//! \param q0: ������ ���ʹϾ� 0. t�� 0.0�� ����� ���� ����Դϴ�.
		//! \param q1: ������ ���ʹϾ� 1. t�� 1.0�� ����� ���� ����Դϴ�.
		//! \param t: ���� ���. [0.0, 1.0]�� ������ �����˴ϴ�.
		//! \return t�� ���� ���� ���� ������ q0�� q1������ ���ʹϾ�
		const Quaternion KEPLER_MATH_API Slerp(const Quaternion& q0, const Quaternion& q1, float t);
	
	}
}