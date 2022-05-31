#pragma once

#include <stdint.h>

#include "Intrinsic.h"

namespace kepler {

	struct Vector4
	{
		union
		{
			struct { float x; float y; float z; float w; };
			struct { float r; float g; float b; float a; };
			float elem[4];
		};
		// Static Variables --------------------------------------
		static const Vector4 Zero;
		static const Vector4 Up;
		static const Vector4 Down;
		static const Vector4 Right;
		static const Vector4 Left;
		static const Vector4 Front;
		static const Vector4 Back;
		//--------------------------------------------------------
		
		// Constructor -------------------------------------------
		Vector4()
			:elem{ 0.0f}
		{}

		Vector4(float _x, float _y, float _z, float _w)
			:x{_x}
			,y{_y}
			,z{_z}
			,w{_w}
		{}

		Vector4(__m128 m)
		{
			_mm_storeu_ps(elem, m);
		}

		Vector4(const Vector4& v) = default;
		Vector4(Vector4&& v) = default;
		//--------------------------------------------------------
		
		// Member Functions --------------------------------------
		__forceinline const float SqLength() const
		{
			__m128 ps = _mm_loadu_ps(elem);
			__m128 result = _mm_mul_ps(ps, ps);
			result = _mm_hadd_ps(result, result);
			result = _mm_hadd_ps(result, result);

			return result.m128_f32[0];
		}

		__forceinline const float Length() const
		{
			const float sqlen = SqLength();
			__m128 result = _mm_sqrt_ps(_mm_set_ps1(sqlen));
		
			return result.m128_f32[0];
		}

		__forceinline const Vector4 Normalize() const
		{

			__m128 ps = _mm_loadu_ps(elem);
			__m128 normalized = _mm_div_ps(ps, _mm_set_ps1(Length()));
			Vector4 result{ normalized };
			return result;
		}
		//--------------------------------------------------------

		// Operator Overloadings ---------------------------------
		__forceinline Vector4& operator=(const Vector4& rhs) 
		{
			__m128 rps = _mm_loadu_ps(rhs.elem);
			__m128 lps = rps;
			_mm_storeu_ps(elem, lps);

			return *this; 
		}

		__forceinline const Vector4 operator+() const
		{ 
			return *this;
		}
		
		__forceinline const Vector4 operator-() const 
		{
			__m128 ps = _mm_loadu_ps(elem);
			
			return Vector4(_mm_mul_ps(ps, _mm_set1_ps(-1.0f)));
		}

		__forceinline float& operator[](const int index) 
		{ 
			return elem[index]; 
		}

		__forceinline const Vector4 operator+(const Vector4& rhs) const 
		{ 
			__m128 lps = _mm_loadu_ps(elem);
			__m128 rps = _mm_loadu_ps(rhs.elem);

			return Vector4(_mm_add_ps(lps, rps));
		}

		__forceinline const Vector4 operator-(const Vector4& rhs) const
		{ 
			__m128 lps = _mm_loadu_ps(elem);
			__m128 rps = _mm_loadu_ps(rhs.elem);

			return Vector4(_mm_sub_ps(lps, rps));
		}

		__forceinline const Vector4 operator*(const Vector4& rhs) const 
		{ 
			__m128 lps = _mm_loadu_ps(elem);
			__m128 rps = _mm_loadu_ps(rhs.elem);
			
			return Vector4(_mm_mul_ps(lps, rps));
		}
		
		__forceinline const Vector4 operator*(const float rhs) const 
		{ 
			__m128 ps = _mm_loadu_ps(elem);
			
			return Vector4(_mm_mul_ps(ps, _mm_set_ps1(rhs)));
		}
		
		__forceinline const Vector4 operator/(const Vector4& rhs) const 
		{
			__m128 lps = _mm_loadu_ps(elem);
			__m128 rps = _mm_loadu_ps(rhs.elem);
			return Vector4(_mm_div_ps(lps, rps));
		}

		__forceinline const Vector4 operator/(const float rhs) const
		{ 
			__m128 ps = _mm_loadu_ps(elem);
			return Vector4(_mm_div_ps(ps, _mm_set_ps1(rhs)));
		}

		__forceinline Vector4& operator+=(const Vector4& rhs) 
		{
			__m128 ps = _mm_loadu_ps(elem);
			__m128 rps = _mm_loadu_ps(rhs.elem);
			ps = _mm_add_ps(ps, rps); 
			_mm_storeu_ps(elem, ps);

			return *this; 
		}

		__forceinline Vector4& operator-=(const Vector4& rhs)
		{ 
			__m128 ps = _mm_loadu_ps(elem);
			__m128 rps = _mm_loadu_ps(rhs.elem);
			ps = _mm_sub_ps(ps, rps); 
			_mm_storeu_ps(elem, ps);

			return *this; 
		}
		__forceinline Vector4& operator*=(const Vector4& rhs) 
		{ 
			__m128 ps = _mm_loadu_ps(elem);
			__m128 rps = _mm_loadu_ps(rhs.elem);
			ps = _mm_mul_ps(ps, rps); 
			_mm_storeu_ps(elem, ps);

			return *this; 
		}
		__forceinline Vector4& operator*=(const float rhs)
		{
			__m128 ps = _mm_loadu_ps(elem);
			ps = _mm_mul_ps(ps, _mm_set_ps1(rhs));
			_mm_storeu_ps(elem, ps);

			return *this;
		}
		__forceinline Vector4& operator/=(const Vector4& rhs)
		{
			__m128 ps = _mm_loadu_ps(elem);
			__m128 rps = _mm_loadu_ps(rhs.elem);
			ps = _mm_div_ps(ps, rps); 
			_mm_storeu_ps(elem, ps);

			return* this; 
		}
		__forceinline Vector4& operator/=(const float rhs) 
		{
			__m128 ps = _mm_loadu_ps(elem);
			ps = _mm_div_ps(ps, _mm_set_ps1(rhs)); 
			_mm_storeu_ps(elem, ps);

			return *this; 
		}
		
		//--------------------------------------------------------

		// Friend Operator Overloadings --------------------------
		__forceinline friend const Vector4 operator*(const float lhs, const Vector4& rhs) 
		{
			return rhs * lhs;
		}
		__forceinline friend const bool operator==(const Vector4& lhs, const Vector4& rhs) 
		{
			__m128 lps = _mm_loadu_ps(lhs.elem);
			__m128 rps = _mm_loadu_ps(rhs.elem);
			return (_mm_movemask_ps(_mm_cmpeq_ps(lps, rps)) & 0b00001111) == 0b00001111; 
		}
		__forceinline friend const bool operator!=(const Vector4& lhs, const Vector4& rhs) { return lhs != rhs; }
		//--------------------------------------------------------
	};

	// Global Functions --------------------------------------
	inline const float Dot(const Vector4& lhs, const Vector4& rhs)
	{
		__m128 lps = _mm_loadu_ps(lhs.elem);
		__m128 rps = _mm_loadu_ps(rhs.elem);

		//ax by cz dw
		__m128 result = _mm_mul_ps(lps, rps);
		// ax+by cz+dw ax+by cz+dw
		result = _mm_hadd_ps(result, result);
		// ax+by+cz+dw ax+by+cz+dw ax+by+cz+dw ax+by+cz+dw
		result = _mm_hadd_ps(result, result);

		return result.m128_f32[0];
	}
	//--------------------------------------------------------

	using Vec4f = Vector4;
}