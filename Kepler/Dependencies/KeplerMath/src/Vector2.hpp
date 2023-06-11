#pragma once

#include <cstdint>
#include <cmath>
#include <cassert>

#include "Base.h"
#include "Constant.h"

namespace kepler {

struct KEPLER_MATH_API Vector2
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
		: x{ _x }
		, y{ _y }
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
		assert((::fabsf(rhs.x) > math::constant::EPSILON) && (::fabsf(rhs.y) > math::constant::EPSILON));
		Vector2 result{ x / rhs.x, y / rhs.y };
		return result;
	}

	inline const Vector2 operator/(const float rhs) const
	{
		assert(::fabsf(rhs) > math::constant::EPSILON);
		Vector2 result{ x / rhs, y / rhs };
		return result;
	}

	inline Vector2& operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
	inline Vector2& operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	inline Vector2& operator*=(const Vector2& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
	inline Vector2& operator*=(const float rhs) { x *= rhs; y *= rhs; return *this; }
	inline Vector2& operator/=(const Vector2& rhs)
	{
		assert((::fabsf(rhs.x) > math::constant::EPSILON) & (::fabsf(rhs.y) > math::constant::EPSILON));

		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}

	inline Vector2& operator/=(const float rhs)
	{
		assert(::fabsf(rhs) > math::constant::EPSILON);

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

struct KEPLER_MATH_API Vector2Int
{
	union
	{
		struct { int32_t x; int32_t y; };
		struct { int32_t u; int32_t v; };
		int32_t elem[2];
	};

	// Static Variables --------------------------------------
	static const Vector2Int Zero;
	static const Vector2Int Up;
	static const Vector2Int Down;
	static const Vector2Int Right;
	static const Vector2Int Left;
	//--------------------------------------------------------

	// Constructor -------------------------------------------
	Vector2Int()
		:elem{ 0 }
	{}

	Vector2Int(const int32_t _x, const int32_t _y)
		: x{ _x }
		, y{ _y }
	{}

	Vector2Int(const int32_t _elem[2])
		:elem{ _elem[0], _elem[1] }
	{}

	Vector2Int(const Vector2Int& v) = default;
	Vector2Int(Vector2Int&& v) = default;
	//--------------------------------------------------------

	// Member Functions --------------------------------------
	inline const int32_t SqLength() const { return x * x + y * y; }
	//--------------------------------------------------------

	// Operator Overloadings ---------------------------------
	inline Vector2Int& operator=(const Vector2Int& rhs) { x = rhs.x; y = rhs.y; return *this; }
	inline int32_t& operator[](int index) { assert(0 <= index && index < 2); return elem[index]; }
	inline const Vector2Int operator+() const { return *this; }
	inline const Vector2Int operator-() const { return Vector2Int{ -x, -y }; }

	inline const Vector2Int operator+(const Vector2Int& rhs) const { Vector2Int result{ x + rhs.x, y + rhs.y }; return result; }
	inline const Vector2Int operator-(const Vector2Int& rhs) const { Vector2Int result{ x - rhs.x, y - rhs.y }; return result; }
	inline const Vector2Int operator*(const Vector2Int& rhs) const { Vector2Int result{ x * rhs.x, y * rhs.y }; return result; }
	inline const Vector2Int operator*(const int32_t rhs) const { Vector2Int result{ x * rhs, y * rhs }; return result; }
	inline const Vector2Int operator/(const Vector2Int& rhs) const { assert(rhs.x != 0 && rhs.y != 0); Vector2Int result{ x / rhs.x, y / rhs.y }; return result; }
	inline const Vector2Int operator/(const int32_t rhs) const { assert(rhs != 0); Vector2Int result{ x / rhs, y / rhs }; return result; }

	inline Vector2Int& operator+=(const Vector2Int& rhs) { x += rhs.x; y += rhs.y; return *this; }
	inline Vector2Int& operator-=(const Vector2Int& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	inline Vector2Int& operator*=(const Vector2Int& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
	inline Vector2Int& operator*=(const int32_t rhs) { x *= rhs; y *= rhs; return *this; }
	inline Vector2Int& operator/=(const Vector2Int& rhs) { assert(rhs.x != 0 && rhs.y != 0); x /= rhs.x; y /= rhs.y; return *this; }
	inline Vector2Int& operator/=(const int32_t rhs) { assert(rhs != 0); x /= rhs; y /= rhs; return *this; }
	//--------------------------------------------------------
	// 
	// Friend Operator Overloadings --------------------------
	inline friend const bool operator==(const Vector2Int& lhs, const Vector2Int& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
	inline friend const bool operator!=(const Vector2Int& lhs, const Vector2Int& rhs) { return !(lhs == rhs); }
	inline friend const Vector2Int operator*(int32_t lhs, const Vector2Int& rhs) { return rhs * lhs; }
	//--------------------------------------------------------
};
// Global Functions --------------------------------------
inline const int32_t Dot(const Vector2Int& lhs, const Vector2Int& rhs) { return (lhs.x * rhs.x + lhs.y * rhs.y); }

//--------------------------------------------------------

using Vec2f = Vector2;
using Vec2i = Vector2Int;
}