#pragma once

#include <cmath>

#include "Intrinsic.h"
#include "MathUtility.h"
#include "Vector3.hpp"

namespace kepler {

	struct Quaternion
	{
		float w;
		union
		{
			struct { float x; float y; float z;  };
			Vec3f v;
		};

		// Static Variables --------------------------------------
		static const Quaternion Identity;
		//--------------------------------------------------------

		// Constructor -------------------------------------------
		Quaternion()
			: x{ 0.0f }
			, y{ 0.0f }
			, z{ 0.0f }
			, w{ 1.0f }
		{}

		Quaternion(const float _x, const float _y, const float _z, const float _w = 1.0f)
			: x{ _x }
			, y{ _y }
			, z{ _z }
			, w{ _w }
		{}

		Quaternion(const Vector3& _v, const float _w = 1.0f)
			: v{ _v }
			, w{ _w }
		{}

		Quaternion(const Quaternion& q) = default;
		Quaternion(Quaternion&& q) = default;
		//--------------------------------------------------------
	
		// Static Member Functions -------------------------------

		// It's implemented on left-handed coordinated.
		// Angles are measured clockwise when looking along the rotation axis toward the origin.
		// The order of transformation is pitch firat, then yaw, then roll.
		static const Quaternion FromEuler(const Vector3& v)
		{
			
			Vector3 angle = v * 0.5f;
			
			float cp = ::cosf(math::DegToRad(angle.x));
			float cy = ::cosf(math::DegToRad(angle.y));
			float cr = ::cosf(math::DegToRad(angle.z));

			float sp = ::sinf(math::DegToRad(angle.x));
			float sy = ::sinf(math::DegToRad(angle.y));
			float sr = ::sinf(math::DegToRad(angle.z));

			Quaternion q = Quaternion::Identity;

			q.w = cp * cy * cr + sp * sy * sr;
			q.x = sp * cy * cr - cp * sy * sr;
			q.y = cp * sy * cr + sp * cy * sr;
			q.z = cp * cy * sr - sp * sy * cr;

			return q;
		}

		static const Quaternion FromAxisAngle(const Vector3& axis, const float angle)
		{
			Quaternion q = Quaternion::Identity;
			float halfRadian = math::DegToRad(angle) / 2.0f;

			q.w = ::cosf(halfRadian);
			q.v = axis * ::sinf(halfRadian);

			return q;
		}

		//--------------------------------------------------------

		// Member Functions --------------------------------------
		const float Length() const { return ::sqrtf(v.SqLength() + w * w); }
		const float SqLength() const { return v.SqLength() + w * w; }
		const Vector3 ToEuler() const 
		{
			Vector3 angle = Vec3f::Zero;
			// pitch(x-axis)
			float sycp = 2.0f * (w * x + y * z);
			float cycp = 1.0f - 2.0f * (x * x + y * y);
			angle.x = ::atan2f(sycp, cycp);

			// yaw(y-axis)
			float sp = 2 * (w * y - z * x);
			if (::fabs(sp) >= 1.0f)
			{
				angle.y = ::copysignf(math::constant::PIDIV2, sp);
			}
			else
			{
				angle.y = ::asinf(sp);
			}

			// roll(z-axis)
			float cpsr = 2.0f * (w * z + x * y);
			float cpcr = 1.0f - 2.0f * (y * y + z * z);
			angle.z = ::atan2f(cpsr, cpcr);

			return angle; 
		};
		

		//--------------------------------------------------------

		// Operator Overloadings ---------------------------------
		Quaternion& operator=(const Quaternion& rhs) { v = rhs.v; w = rhs.w; return *this; }
		const Quaternion operator+() const { return *this; }
		const Quaternion operator-() const { return Quaternion{ -v }; }

		const Quaternion operator+(const Quaternion& rhs) const { Quaternion result{ v + rhs.v, w + rhs.w }; return result; }
		const Quaternion operator-(const Quaternion& rhs) const { Quaternion result{ v - rhs.v, w + rhs.w }; return result; }
		const Quaternion operator*(const float rhs) const { Quaternion result{ v * rhs, w * rhs }; return result; }
		const Quaternion operator/(const float rhs) const { Quaternion result{ v / rhs, w / rhs }; return result; }

		const Quaternion operator*(const Quaternion& rhs) const { Quaternion result{ w * rhs.v + rhs.w * v + Cross(v, rhs.v), w * rhs.w - Dot(v, rhs.v) }; return result; }
		
		Quaternion& operator+=(const Quaternion& rhs) { v += rhs.v; w += rhs.w; return *this; }
		Quaternion& operator-=(const Quaternion& rhs) { v -= rhs.v; w -= rhs.w; return *this; }
		Quaternion& operator*=(const float rhs) { v *= rhs; w *= rhs; return *this; }

		Quaternion& operator*=(const Quaternion& rhs) { float tempW = w; w = tempW * rhs.w - Dot(v, rhs.v); v = tempW * rhs.v + rhs.w * v + Cross(v, rhs.v); return *this; }
		//--------------------------------------------------------

		// Friend Operator Overloadings --------------------------
		friend const Quaternion operator*(const float lhs, const Quaternion& rhs) { return rhs * lhs; }
		friend const bool operator==(const Quaternion& lhs, const Quaternion& rhs) { return lhs.w == rhs.w && lhs.v == rhs.v; }
		friend const bool operator!=(const Quaternion& lhs, const Quaternion& rhs) { return !(lhs == rhs); }

		//--------------------------------------------------------
	};
}