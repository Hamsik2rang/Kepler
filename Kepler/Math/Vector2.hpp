#pragma once

#include <cstdint>
#include <cmath>
#include <cassert>

#include "Core/Base.h"
#include "Math/Constant.h"

namespace kepler {

	struct Vector2
	{
		union
		{
			struct { float x; float y; };
			struct { float u; float v; };
			float elem[2];
		};
		// Static Variables --------------------------------------
		static const Vector2 Zero;
		static const Vector2 Up;
		static const Vector2 Down;
		static const Vector2 Right;
		static const Vector2 Left;
		//--------------------------------------------------------

		// Constructor -------------------------------------------
		Vector2()
			:elem{ 0.0f }
		{}

		Vector2(const float _x, const float _y)
			: x{_x}
			, y{_y}
		{}

		Vector2(const float _elem[2])
			:elem{ _elem[0], _elem[1] }
		{}

		Vector2(const Vector2& v) = default;
		Vector2(Vector2&& v) = default;
		//--------------------------------------------------------
	
		// Member Functions --------------------------------------
		inline const float Length() const { return ::sqrtf(x * x + y * y); }
		inline const float SqLength() const { return x * x + y * y; }
		inline const Vector2 Normalize() const { Vector2 result{ x / Length(), y / Length() }; return result; }
		//--------------------------------------------------------
		
		// Operator Overloadings ---------------------------------
		inline Vector2& operator=(const Vector2& rhs) { x = rhs.x; y = rhs.y; return *this; }
		inline float& operator[](int index) { assert(0 <= index && index < 2); return elem[index]; }
		inline const Vector2 operator+() const { return *this; }
		inline const Vector2 operator-() const { Vector2 result{ x * -1.0f, y * -1.0f }; return result; }

		inline const Vector2 operator+(const Vector2& rhs) const { Vector2 result{ x + rhs.x, y + rhs.y }; return result; }
		inline const Vector2 operator-(const Vector2& rhs) const { Vector2 result{ x - rhs.x, y - rhs.y }; return result; }
		inline const Vector2 operator*(const Vector2& rhs) const { Vector2 result{ x * rhs.x, y * rhs.y }; return result; }
		inline const Vector2 operator*(const float rhs) const { Vector2 result{ x * rhs, y * rhs }; return result; }
		inline const Vector2 operator/(const Vector2& rhs) const 
		{ 
			KEPLER_ASSERT_NO_MESSAGE((::fabsf(rhs.x) > math::constant::EPSILON) && (::fabsf(rhs.y) > math::constant::EPSILON));
			Vector2 result{ x / rhs.x, y / rhs.y }; 
			return result; 
		}

		inline const Vector2 operator/(const float rhs) const 
		{
			KEPLER_ASSERT_NO_MESSAGE(::fabsf(rhs) > math::constant::EPSILON);
			Vector2 result{ x / rhs, y / rhs }; 
			return result; 
		}

		inline Vector2& operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
		inline Vector2& operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
		inline Vector2& operator*=(const Vector2& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
		inline Vector2& operator*=(const float rhs) { x *= rhs; y *= rhs; return *this; }
		inline Vector2& operator/=(const Vector2& rhs)
		{
			KEPLER_ASSERT_NO_MESSAGE((::fabsf(rhs.x) > math::constant::EPSILON) & (::fabsf(rhs.y) > math::constant::EPSILON));
			
			x /= rhs.x;
			y /= rhs.y; 
			return *this; 
		}

		inline Vector2& operator/=(const float rhs) 
		{
			KEPLER_ASSERT_NO_MESSAGE(::fabsf(rhs) > math::constant::EPSILON);
			
			x /= rhs; 
			y /= rhs; 
			return *this; 
		}
		//--------------------------------------------------------

		// Friend Operator Overloadings --------------------------
		inline friend const bool operator==(const Vector2& lhs, const Vector2& rhs) 
		{
			return (::fabsf(lhs.x - rhs.x) < math::constant::EPSILON) 
				&& (::fabsf(lhs.y - rhs.y) < math::constant::EPSILON); 
		}
		inline friend const bool operator!=(const Vector2& lhs, const Vector2& rhs) { return !(lhs == rhs); }
		inline friend const Vector2 operator*(const float lhs, const Vector2& rhs) { return rhs * lhs; }
		//--------------------------------------------------------
	};

	// Global Functions --------------------------------------
	inline const float Dot(const Vector2& lhs, const Vector2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; };
	
	//--------------------------------------------------------

	using Vec2f = Vector2;
}