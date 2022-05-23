#pragma once

#include <stdint.h>

#include "Intrinsic.h"

namespace kepler {

	struct Vector4;
	struct Vector4Int;

	using Vec4f = Vector4;
	using Vec4i = Vector4Int;

	struct alignas(16) Vector4
	{
		union
		{
			struct { float x; float y; float z; float w; };
			struct { float r; float g; float b; float a; };
			float elem[4];
			__m128 ps;
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
			:ps{ m }
		{}

		Vector4(const Vector4& v) = default;
		Vector4(Vector4&& v) = default;
		//--------------------------------------------------------
		
		// Member Functions --------------------------------------
		__forceinline const float SqLength() const
		{
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
			__m128 result = _mm_div_ps(ps, _mm_set_ps1(Length()));
			
			return result;
		}
		//--------------------------------------------------------

		// Operator Overloadings ---------------------------------
		__forceinline Vector4& operator=(const Vector4& rhs) { ps = rhs.ps; return *this; }
		__forceinline const Vector4 operator+() const { return *this; }
		__forceinline const Vector4 operator-() const { return _mm_mul_ps(ps, _mm_set1_ps(-1.0f)); }
		__forceinline float& operator[](const int index) { return elem[index]; }
		__forceinline const Vector4 operator+(const Vector4& rhs) const { return _mm_add_ps(ps, rhs.ps); }
		__forceinline const Vector4 operator-(const Vector4& rhs) const { return _mm_sub_ps(ps, rhs.ps); }
		__forceinline const Vector4 operator*(const Vector4& rhs) const { return _mm_mul_ps(ps, rhs.ps); }
		__forceinline const Vector4 operator*(const float rhs) const { return _mm_mul_ps(ps, _mm_set_ps1(rhs)); }
		__forceinline const Vector4 operator/(const Vector4& rhs) const { return _mm_div_ps(ps, rhs.ps); }
		__forceinline const Vector4 operator/(const float rhs) const { return _mm_div_ps(ps, _mm_set_ps1(rhs)); }

		__forceinline Vector4& operator+=(const Vector4& rhs) { ps = _mm_add_ps(ps, rhs.ps); return *this; }
		__forceinline Vector4& operator-=(const Vector4& rhs) { ps = _mm_sub_ps(ps, rhs.ps); return *this; }
		__forceinline Vector4& operator*=(const Vector4& rhs) { ps = _mm_mul_ps(ps, rhs.ps); return *this; }
		__forceinline Vector4& operator*=(const float rhs) { ps = _mm_mul_ps(ps, _mm_set_ps1(rhs)); return *this; }
		__forceinline Vector4& operator/=(const Vector4& rhs) { ps = _mm_div_ps(ps, rhs.ps); return* this; }
		__forceinline Vector4& operator/=(const float rhs) { ps = _mm_div_ps(ps, _mm_set_ps1(rhs)); return *this; }
		
		inline void* operator new(size_t size) { void* p = _aligned_malloc(size, 16); return p; }
		inline void operator delete(void* p) { _aligned_free(p); }
		//--------------------------------------------------------

		// Friend Operator Overloadings --------------------------
		__forceinline friend const Vector4 operator*(const float lhs, const Vector4& rhs) { return _mm_mul_ps(rhs.ps, _mm_set_ps1(lhs)); }
		__forceinline friend const bool operator==(const Vector4& lhs, const Vector4& rhs) { return (_mm_movemask_ps(_mm_cmpeq_ps(lhs.ps, rhs.ps)) & 0b00001111) == 0b00001111; }
		__forceinline friend const bool operator!=(const Vector4& lhs, const Vector4& rhs) { return lhs != rhs; }
		//--------------------------------------------------------
	};

	// Static Variables --------------------------------------
	const Vector4 Vector4::Zero		= { 0.0f, 0.0f, 0.0f, 0.0f };
	const Vector4 Vector4::Up		= { 0.0f, 1.0f, 0.0f, 0.0f };
	const Vector4 Vector4::Down		= { 0.0f, -1.0f, 0.0f, 0.0f };
	const Vector4 Vector4::Right	= { 1.0f, 0.0f, 0.0f, 0.0f };
	const Vector4 Vector4::Left		= { -1.0f, 0.0f, 0.0f, 0.0f };
	const Vector4 Vector4::Front	= { 0.0f, 0.0f, 1.0f, 0.0f };
	const Vector4 Vector4::Back		= { 0.0f, 0.0f, -1.0f, 0.0f };
	//--------------------------------------------------------

	// Global Functions --------------------------------------
	const float Dot(const Vector4& lhs, const Vector4& rhs)
	{
		//ax by cz dw
		__m128 result = _mm_mul_ps(lhs.ps, rhs.ps);
		// ax+by cz+dw ax+by cz+dw
		result = _mm_hadd_ps(result, result);
		// ax+by+cz+dw ax+by+cz+dw ax+by+cz+dw ax+by+cz+dw
		result = _mm_hadd_ps(result, result);

		return result.m128_f32[0];
	}
	//--------------------------------------------------------
}