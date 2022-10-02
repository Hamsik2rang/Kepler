#pragma once

#include "Intrinsic.h"
#include "Matrix.h"
#include "Vector.h"
#include "Quaternion.hpp"
#include "MathUtility.h"

namespace kepler {
	namespace math {

		//! �̵� ��ȯ ����� �����մϴ�.
		//! \author Im Yongsik
		//! \param position: �̵��� ���� ǥ���ϴ� 3���� ����
		//! \return 4x4 �̵� ��ȯ ���
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

		//! x�� ȸ�� ��ȯ ����� �����մϴ�.
		//! \author Im Yongsik
		//! \param angle: x�� ���� �ð� ���� ȸ������ ��Ÿ���� ���Ȱ�
		//! \return 4x4 ȸ�� ��ȯ ���
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

		//! y�� ȸ�� ��ȯ ����� �����մϴ�.
		//! \author Im Yongsik
		//! \param angle: y�� ���� �ð� ���� ȸ������ ��Ÿ���� ���Ȱ�
		//! \return 4x4 ȸ�� ��ȯ ���
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

		//! z�� ȸ�� ��ȯ ����� �����մϴ�.
		//! \author Im Yongsik
		//! \param angle: z�� ���� �ð� ���� ȸ������ ��Ÿ���� ���Ȱ�
		//! \return 4x4 ȸ�� ��ȯ ���
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
		
		//! ȸ�� ��ȯ ����� �����մϴ�.
		//! \author Im Yongsik
		//! \param rotation: ȸ�� ���ʹϾ�
		//! \return 4x4 ȸ�� ��ȯ ���
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
		//! ���� ��ȯ ����� �����մϴ�.
		//! \author Im Yongsik
		//! \param scale: ������ x, y, z�� ���� ���/Ȯ��� ũ�⸦ ��Ÿ���� 3���� ����
		//! \return scale��ŭ �������� ���� ���/Ȯ���Ű�� 4x4 ��ȯ ���
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
		//! ���� �������� ī�޶� ������ �̿��� ��(ī�޶�) ��ȯ ����� �����մϴ�.
		//! \author Im Yongsik
		//! \param eye: ī�޶��� ��ġ
		//! \param at: ī�޶��� ����(ī�޶� �ٶ󺸴� ����)
		//! \param worldUp: ī�޶� ��ġ�� ���� ������ y�� ��������
		//! \return 4x4 �� ��ȯ ���
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
		//! ���� ���� ����� �����մϴ�.
		//! \author Im Yongsik
		//! \param fovY: y�� ���� �þ߰�(Field of View)
		//! \param aspect: ����ȭ�Ϸ��� �� ������ ����/���� ����
		//! \param nearClip: �ּ� ���� ����(near clip plane)
		//! \param farClip: �ִ� ���� ����(far clip plane
		//! \return 4x4 ���� ���� ���
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
		//! ���� ���� ����� �����մϴ�.
		//! \author Im Yongsik
		//! \param left: ����ȭ�Ϸ��� �� ������ �ּ� x��ǥ
		//! \param right: ����ȭ�Ϸ��� �� ������ �ִ� x��ǥ
		//! \param top: ����ȭ�Ϸ��� �� ������ �ּ� y��ǥ
		//! \param bottom: ����ȭ�Ϸ��� �� ������ �ִ� y��ǥ
		//! \param nearClip: �ּ� ���� ����.
		//! \param farClip: �ִ� ���� ����. 
		//! \return 4x4 ���� ���� ���
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

		//! ȸ�� ���ʹϾ��� ���� 3���� ���͸� ȸ���մϴ�.
		//! \author Im Yongsik
		//! \param q ȸ�� ���ʹϾ�
		//! \param v ȸ���ϰ��� �ϴ� ����
		//! \return v�� q�� ȸ���� ��� ����
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