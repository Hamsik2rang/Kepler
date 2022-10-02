#pragma once

#include "Intrinsic.h"
#include "Matrix.h"
#include "Vector.h"
#include "Quaternion.hpp"
#include "MathUtility.h"

namespace kepler {
	namespace math {

		//! 이동 변환 행렬을 생성합니다.
		//! \author Im Yongsik
		//! \param position: 이동할 양을 표현하는 3차원 벡터
		//! \return 4x4 이동 변환 행렬
		inline const Mat44f GetTranslateMatrix(const Vector3& position)
		{
			Mat44f T {
				1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				position.x, position.y, position.z, 1.0f
			};

			return T;
		}

		//! x축 회전 변환 행렬을 생성합니다.
		//! \author Im Yongsik
		//! \param angle: x축 기준 시계 방향 회전량을 나타내는 라디안각
		//! \return 4x4 회전 변환 행렬
		inline const Mat44f GetRotationMatrixX(const float angle)
		{
			Mat44f Rx {
				1.0f, 0.0f,				0.0f,			0.0f,
				0.0f, ::cosf(angle),	::sinf(angle),	0.0f,
				0.0f, -::sinf(angle),	cosf(angle),	0.0f,
				0.0f, 0.0f,				0.0f,			1.0f
			};

			return Rx;
		}

		//! y축 회전 변환 행렬을 생성합니다.
		//! \author Im Yongsik
		//! \param angle: y축 기준 시계 방향 회전량을 나타내는 라디안각
		//! \return 4x4 회전 변환 행렬
		inline const Mat44f GetRotationMatrixY(const float angle)
		{
			Mat44f Ry {
				::cosf(angle),  0.0f, -::sinf(angle),	0.0f,
				0.0f,			1.0f, 0.0f,				0.0f,
				::sinf(angle),	0.0f, ::cosf(angle),	0.0f,
				0.0f,			0.0f, 0.0f,				1.0f
			};

			return Ry;
		}

		//! z축 회전 변환 행렬을 생성합니다.
		//! \author Im Yongsik
		//! \param angle: z축 기준 시계 방향 회전량을 나타내는 라디안각
		//! \return 4x4 회전 변환 행렬
		inline const Mat44f GetRotationMatrixZ(const float angle)
		{
			Mat44f Rz {
				::cosf(angle),	::sinf(angle),	0.0f, 0.0f,
				-::sinf(angle),	::cosf(angle),	0.0f, 0.0f,
				0.0f,			0.0f,			1.0f, 0.0f,
				0.0f,			0.0f,			0.0f, 1.0f
			};

			return Rz;
		}
		
		//! 회전 변환 행렬을 생성합니다.
		//! \author Im Yongsik
		//! \param rotation: 회전 쿼터니언
		//! \return 4x4 회전 변환 행렬
		inline const Mat44f GetRotationMatrix(const Quaternion& rotation)
		{
			const Quaternion& q = rotation;
			Mat44f R {
				1.0f - 2.0f * (q.y * q.y + q.z * q.z), 2.0f * (q.x * q.y - q.w * q.z), 2.0f * (q.w * q.y + q.x * q.z),	0.0f,
				2.0f * (q.x * q.y + q.w * q.z),	1.0f - 2.0f * (q.x * q.x + q.z * q.z), 2.0f * (q.y * q.z - q.w * q.x),	0.0f,
				2.0f * (q.x * q.z - q.w * q.y), 2.0f * (q.w * q.x + q.y * q.z),	1.0f - 2.0f * (q.x * q.x - q.y * q.y), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};

			return R;
		}
		//! 신축 변환 행렬을 생성합니다.
		//! \author Im Yongsik
		//! \param scale: 기저축 x, y, z를 따라 축소/확대될 크기를 나타내는 3차원 벡터
		//! \return scale만큼 기준축을 따라 축소/확대시키는 4x4 변환 행렬
		inline const Mat44f GetScalingMatrix(const Vector3& scale)
		{
			Mat44f scaling {
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
		//! 월드 공간상의 카메라 정보를 이용해 뷰(카메라) 변환 행렬을 생성합니다.
		//! \author Im Yongsik
		//! \param eye: 카메라의 위치
		//! \param at: 카메라의 초점(카메라가 바라보는 지점)
		//! \param worldUp: 카메라가 위치한 월드 공간의 y축 단위벡터
		//! \return 4x4 뷰 변환 행렬
		inline const Mat44f LookAt(const Vector3& eye, const Vector3& at, const Vector3& worldUp = { 0.0f, 1.0f, 0.0f })
		{
			Vec3f front = (at - eye).Normalize();

			Vec3f right = Cross(worldUp, front).Normalize();
			Vec3f up = Cross(front, right);

			float tr = Dot(-eye, right);
			float tu = Dot(-eye, up);
			float tf = Dot(-eye, front);

			Mat44f view{
				right.x,	up.x,	front.x,	0.0f,
				right.y,	up.y,	front.y,	0.0f,
				right.z,	up.z,	front.z,	0.0f,
				tr,			tu,		tf,			1.0f
			};

			return view;
		}
		//! 원근 투영 행렬을 생성합니다.
		//! \author Im Yongsik
		//! \param fovY: y축 기준 시야각(Field of View)
		//! \param aspect: 정규화하려는 뷰 볼륨의 가로/세로 비율
		//! \param nearClip: 최소 투영 깊이(near clip plane)
		//! \param farClip: 최대 투영 깊이(far clip plane
		//! \return 4x4 원근 투영 행렬
		inline const Mat44f Perspective(const float fovY, const float aspect, const float nearClip, const float farClip)
		{
			Mat44f perspective {
			1.0f / (std::tanf(DegToRad(fovY / 2.0f)) * aspect), 0.0f, 0.0f, 0.0f ,
			0.0f, 1.0f / std::tanf(DegToRad(fovY / 2.0f)), 0.0f, 0.0f ,
			0.0f, 0.0f, farClip / (farClip - nearClip), 1.0f,
			0.0f, 0.0f, -nearClip * farClip / (farClip - nearClip), 0.0f
			};

			return perspective;
		}
		//! 직교 투영 행렬을 생성합니다.
		//! \author Im Yongsik
		//! \param left: 정규화하려는 뷰 볼륨의 최소 x좌표
		//! \param right: 정규화하려는 뷰 볼륨의 최대 x좌표
		//! \param top: 정규화하려는 뷰 볼륨의 최소 y좌표
		//! \param bottom: 정규화하려는 뷰 볼륨의 최대 y좌표
		//! \param nearClip: 최소 투영 깊이.
		//! \param farClip: 최대 투영 깊이. 
		//! \return 4x4 직교 투영 행렬
		inline const Mat44f Orthographic(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip)
		{
			Mat44f orthographic {
				2.0f / (right - left), 0.0f, 0.0f, 0.0f,
				0.0f, 2.0f / (bottom - top), 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f / (farClip - nearClip), 0.0f,
				0.0f, 0.0f, -1.0f / (farClip - nearClip) * nearClip, 1.0f
			};

			return orthographic;
		}

		//! 회전 쿼터니언을 통해 3차원 벡터를 회전합니다.
		//! \author Im Yongsik
		//! \param q 회전 쿼터니언
		//! \param v 회전하고자 하는 벡터
		//! \return v를 q로 회전한 결과 벡터
		inline const Vec3f Rotate(const Quat& q, const Vec3f& v)
		{
			Quat vq{ v.x, v.y, v.z, 0.0f };
			Quat result = q * vq;
			Quat cq = q.Conjugate();
			result = result * q.Conjugate();

			return Vec3f(result.x, result.y, result.z);
		}
	}
}