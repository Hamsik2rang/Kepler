#pragma once

#include "Intrinsic.h"
#include "Matrix.h"
#include "Vector.h"
#include "Quaternion.h"
#include "MathUtility.h"

namespace kepler {
	namespace math {

		inline const Mat44f GetTranslateMatrix(const Vector3& position)
		{
			Mat44f T{
				1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				position.x, position.y, position.z, 1.0f
			};

			return T;
		}

		inline const Mat44f GetRotationMatrixX(const float angle)
		{
			Mat44f Rx{
				1.0f, 0.0f,				0.0f,			0.0f,
				0.0f, ::cosf(angle),	::sinf(angle),	0.0f,
				0.0f, -::sinf(angle),	cosf(angle),	0.0f,
				0.0f, 0.0f,				0.0f,			1.0f
			};

			return Rx;
		}

		inline const Mat44f GetRotationMatrixY(const float angle)
		{
			Mat44f Ry{
				::cosf(angle),  0.0f, -::sinf(angle),	0.0f,
				0.0f,			1.0f, 0.0f,				0.0f,
				::sinf(angle),	0.0f, ::cosf(angle),	0.0f,
				0.0f,			0.0f, 0.0f,				1.0f
			};

			return Ry;
		}

		inline const Mat44f GetRotationMatrixZ(const float angle)
		{
			Mat44f Rz{
				::cosf(angle),	::sinf(angle),	0.0f, 0.0f,
				-::sinf(angle),	::cosf(angle),	0.0f, 0.0f,
				0.0f,			0.0f,			1.0f, 0.0f,
				0.0f,			0.0f,			0.0f, 1.0f
			};

			return Rz;
		}

		inline const Mat44f GetRotationMatrix(const Quaternion& rotation)
		{
			const Quaternion& q = rotation;
			Mat44f R = {
				1.0f - 2.0f * (q.y * q.y + q.z * q.z), 2.0f * (q.x * q.y - q.w * q.z), 2.0f * (q.w * q.y + q.x * q.z),	0.0f,
				2.0f * (q.x * q.y + q.w * q.z),	1.0f - 2.0f * (q.x * q.x + q.z * q.z), 2.0f * (q.y * q.z - q.w * q.x),	0.0f,
				2.0f * (q.x * q.z - q.w * q.y), 2.0f * (q.w * q.x + q.y * q.z),	1.0f - 2.0f * (q.x * q.x - q.y * q.y), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};

			return R;
		}

		inline const Mat44f GetScalingMatrix(const Vector3& scale)
		{
			Mat44f scaling{
				scale.x,	0.0f,		0.0f,		0.0f,
				0.0f,		scale.y,	0.0f,		0.0f,
				0.0f,		0.0f,		scale.z,	0.0f,
				0.0f,		0.0f,		0.0f,		1.0f
			};

			return scaling;
		}

		inline const Mat44f GetWorldMatrix(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
		{
			return GetScalingMatrix(scale) * GetRotationMatrix(rotation) * GetTranslateMatrix(position);
		}

		inline const Mat44f LookAt(const Vector3& eye, const Vector3& at, const Vector3& worldUp = { 0.0f, 1.0f, 0.0f })
		{
			Vec3f right = Cross(worldUp, at).Normalize();
			Vec3f up = Cross(at, right).Normalize();
			Vec3f front = Cross(right, up);

			Mat44f view{
				right.x,	up.x,	front.z,	0.0f,
				right.y,	up.y,	front.z,	0.0f,
				right.z,	up.z,	front.z,	0.0f,
				-eye.x,		-eye.y,	-eye.z,		1.0f
			};

			return view;
		}

		inline const Mat44f Perspective(const float fovY, const float aspect, const float nearClip, const float farClip)
		{
			Mat44f perspective{
			1.0f / (std::tanf(DegToRad(fovY / 2.0f)) * aspect), 0.0f, 0.0f, 0.0f ,
			0.0f, 1.0f / std::tanf(DegToRad(fovY / 2.0f)), 0.0f, 0.0f ,
			0.0f, 0.0f, farClip / (farClip - nearClip), -1.0f,
			0.0f, 0.0f, nearClip * farClip / (farClip - nearClip), 0.0f
			};

			return perspective;
		}

		inline const Mat44f Orthographic(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip)
		{
			Mat44f orthographic{
				2.0f / (right - left), 0.0f, 0.0f, 0.0f,
				0.0f, 2.0f / (bottom - top), 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f / (farClip - nearClip), 0.0f,
				0.0f, 0.0f, -1.0f / (farClip - nearClip) * nearClip, 1.0f
			};

			return orthographic;
		}
	}
}