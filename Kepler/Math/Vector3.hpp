#pragma once

#include <stdint.h>
#include <cstring>

#include "Intrinsic.h"
#include "Vector4.hpp"

namespace kepler {

	struct Vector3;

	void StoreM128ToVec3(Vector3& v, const __m128& ps);

	struct Vector3
	{
		union
		{
			struct { float x; float y; float z; };
			struct { float r; float g; float b; };
			float elem[3];
		};
		// Static Variables --------------------------------------
		static const Vector3 Zero;
		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Right;
		static const Vector3 Left;
		static const Vector3 Front;
		static const Vector3 Back;
		//--------------------------------------------------------

		// Constructor -------------------------------------------
		Vector3()
			:x{ 0.0f }
			,y{ 0.0f }
			,z{ 0.0f }
		{}

		Vector3(const float _x, const float _y, const float _z)
			:x{ _x }
			,y{ _y }
			,z{ _z }		
		{}

		Vector3(const float(&_elem)[3])
			:elem{ _elem[0], _elem[1], _elem[2] }
		{}
		//--------------------------------------------------------

		// Member Functions --------------------------------------
		__forceinline const __m128 ToM128() const
		{
			__m128 zero = _mm_set1_ps(0.0f);
			__m128 xy = _mm_loadl_pi(zero, reinterpret_cast<const __m64*>(&x));
			__m128 z0 = _mm_load_ss(&z);
			__m128 ps = _mm_movelh_ps(xy, z0);

			return ps;
		}

		__forceinline const float SqLength() const
		{
			__m128 ps = ToM128();

			__m128 mul = _mm_mul_ps(ps, ps);
			mul = _mm_hadd_ps(mul, mul);
			mul = _mm_hadd_ps(mul, mul);

			return _mm_cvtss_f32(mul);
		}

		__forceinline const float Length() const
		{
			const float sqlen = SqLength();
			__m128 ps = _mm_sqrt_ss(_mm_set1_ps(sqlen));
			
			return _mm_cvtss_f32(ps);
		}

		__forceinline const Vector3 Normalize() const
		{
			__m128 ps = ToM128();

			float len = Length();
			ps = _mm_div_ps(ps, _mm_set1_ps(len));
			Vector3 result;
			StoreM128ToVec3(result, ps);

			return result;
		}

		__forceinline const Vector4 ToAffine() const
		{
			__m128 xyz0 = ToM128();
			__m128 xz11 = _mm_shuffle_ps(xyz0, _mm_set1_ps(1.0f), KP_MM_SHUFFLE_LE(0, 2, 0, 0));
			__m128 result = _mm_shuffle_ps(xyz0, xz11, KP_MM_SHUFFLE_LE(0, 1, 1, 2));
		
			return result;
		}
		//--------------------------------------------------------

		// Operator Overloadings ---------------------------------
		__forceinline float& operator[](int index) { /*Need assert*/ return elem[index]; }
		__forceinline const float operator[](int index) const { return elem[index]; }
		__forceinline const Vector3 operator+() const { return *this; }
		__forceinline const Vector3 operator-() const 
		{
			__m128 ps = ToM128();
			ps =  _mm_xor_ps(ps, _mm_castsi128_ps(_mm_set1_epi32(0x80'00'00'00)));
			Vector3 result;
			StoreM128ToVec3(result, ps);
			
			return result;
		}

		__forceinline const Vector3 operator+(const Vector3& rhs) const
		{
			__m128 lps = ToM128();
			__m128 rps = rhs.ToM128();
			__m128 add = _mm_add_ps(lps, rps);
			Vector3 result;
			StoreM128ToVec3(result, add);

			return result;
		}
		
		__forceinline const Vector3 operator-(const Vector3& rhs) const
		{
			__m128 lps = ToM128();
			__m128 rps = rhs.ToM128();
			__m128 sub = _mm_sub_ps(lps, rps);
			Vector3 result;
			StoreM128ToVec3(result, sub);

			return result;
		}

		__forceinline const Vector3 operator*(const Vector3& rhs) const
		{
			__m128 lps = ToM128();
			__m128 rps = rhs.ToM128();
			__m128 mul = _mm_mul_ps(lps, rps);

			Vector3 result;
			StoreM128ToVec3(result, mul);

			return result;
		}

		__forceinline const Vector3 operator*(const float rhs) const
		{
			__m128 ps = ToM128();
			__m128 mul = _mm_mul_ps(ps, _mm_set_ps1(rhs));
			Vector3 result;
			StoreM128ToVec3(result, mul);

			return result;
		}

		__forceinline const Vector3 operator/(const Vector3& rhs) const
		{
			__m128 lps = ToM128();
			__m128 rps = rhs.ToM128();
			__m128 div = _mm_div_ps(lps, rps);

			Vector3 result;
			StoreM128ToVec3(result, div);

			return result;
		}

		__forceinline const Vector3 operator/(const float rhs) const
		{
			__m128 ps = ToM128();
			__m128 div = _mm_div_ps(ps, _mm_set_ps1(rhs));
			
			Vector3 result;
			StoreM128ToVec3(result, div);
			
			return result;
		}

		__forceinline Vector3& operator=(const Vector3& rhs)
		{
			memcpy(this, &rhs.x, sizeof(Vector3));
			
			return *this;
		}

		__forceinline Vector3& operator+=(const Vector3& rhs)
		{
			__m128 lps = ToM128();
			__m128 rps = rhs.ToM128();
			__m128 add = _mm_add_ps(lps, rps);

			StoreM128ToVec3(*this, add);

			return *this;
		}

		__forceinline Vector3& operator-=(const Vector3& rhs)
		{
			__m128 lps = ToM128();
			__m128 rps = rhs.ToM128();
			__m128 sub = _mm_sub_ps(lps, rps);

			StoreM128ToVec3(*this, sub);

			return *this;
		}

		__forceinline Vector3& operator*=(const Vector3& rhs)
		{
			__m128 lps = ToM128();
			__m128 rps = rhs.ToM128();
			__m128 mul = _mm_mul_ps(lps, rps);

			StoreM128ToVec3(*this, mul);

			return *this;
		}

		__forceinline Vector3& operator*=(const float rhs)
		{
			__m128 ps = ToM128();
			__m128 mul = _mm_mul_ps(ps, _mm_set_ps1(rhs));

			StoreM128ToVec3(*this, mul);

			return *this;
		}

		__forceinline Vector3& operator/=(const Vector3& rhs)
		{
			__m128 lps = ToM128();
			__m128 rps = rhs.ToM128();
			__m128 div = _mm_div_ps(lps, rps);

			StoreM128ToVec3(*this, div);

			return *this;
		}

		__forceinline Vector3& operator/=(const float rhs)
		{
			__m128 ps = ToM128();
			__m128 div = _mm_div_ps(ps, _mm_set_ps1(rhs));
			StoreM128ToVec3(*this, div);

			return *this;
		}
		//--------------------------------------------------------

		// Friend Operator Overloadings --------------------------
		__forceinline friend const Vector3 operator*(const float lhs, const Vector3& rhs)
		{
			return rhs * lhs;
		}

		__forceinline friend const bool operator==(const Vector3& lhs, const Vector3& rhs)
		{
			__m128 lps = lhs.ToM128();
			__m128 rps = rhs.ToM128();

			return (_mm_movemask_ps(_mm_cmpeq_ps(lps, rps)) & 0b00001111) == 0b00001111;
		}

		__forceinline friend const bool operator!=(const Vector3& lhs, const Vector3& rhs)
		{
			return !(lhs == rhs);
		}
		//--------------------------------------------------------
	};

	// Global Functions --------------------------------------
	inline const Vector3 Cross(const Vector3& lhs, const Vector3& rhs)
	{
		__m128 lps = lhs.ToM128();
		__m128 rps = rhs.ToM128();
		__m128 lmul = _mm_mul_ps(_mm_shuffle_ps(lps, lps, KP_MM_SHUFFLE_LE(1, 2, 0, 3)), _mm_shuffle_ps(rps, rps, KP_MM_SHUFFLE_LE(2, 0, 1, 3)));
		__m128 rmul = _mm_mul_ps(_mm_shuffle_ps(lps, lps, KP_MM_SHUFFLE_LE(2, 0, 1, 3)), _mm_shuffle_ps(rps, rps, KP_MM_SHUFFLE_LE(1, 2, 0, 3)));
		__m128 cross = _mm_sub_ps(lmul, rmul);

		return Vector3(cross.m128_f32[0], cross.m128_f32[1], cross.m128_f32[2]);
	}

	inline const float Dot(const Vector3& lhs, const Vector3& rhs)
	{
		__m128 zero = _mm_set1_ps(0.0f);
		__m128 lxy = _mm_loadl_pi(zero, reinterpret_cast<const __m64*>(&lhs.x));
		__m128 rxy = _mm_loadl_pi(zero, reinterpret_cast<const __m64*>(&rhs.x));

		__m128 lz0 = _mm_load_ss(&lhs.z);
		__m128 rz0 = _mm_load_ss(&rhs.z);

		__m128 lps = _mm_movelh_ps(lxy, lz0);
		__m128 rps = _mm_movelh_ps(rxy, rz0);

		__m128 result = _mm_mul_ps(lps, rps);
		result = _mm_hadd_ps(result, result);
		result = _mm_hadd_ps(result, result);

		return _mm_cvtss_f32(result);
	}

	__forceinline void StoreM128ToVec3(Vector3& v, const __m128& ps)
	{
		_mm_storel_pi(reinterpret_cast<__m64*>(&v.x), ps);
		__m128 z0z0 = _mm_movehl_ps(ps, ps);
		_mm_store_ss(&v.z, z0z0);
	}
	//--------------------------------------------------------

	using Vec3f = Vector3;
}
