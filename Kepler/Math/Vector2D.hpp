#pragma once

// for SIMD, not necessary now
//#include <intrin.h>
#define _VECTOR_2D_

#include <type_traits>
#include <cmath>

#include "Core/Base.h"

namespace kepler {
	namespace math {

		template <typename T> struct Vector2D;
		template <typename T> struct Vector3D;
		template <typename T> struct Vector4D;

		using Vec2f = Vector2D<float>;			// Vector2D of float
		using Vec2i = Vector2D<int>;			// Vector2D of int
		using Vec2c = Vector2D<char>;			// Vector2D of char
		using Vec2b = Vector2D<unsigned char>;	// Vector2D of byte(uchar)

		template <typename T>
		struct /*alignas(16)*/ Vector2D
		{
			
			union
			{
				struct { T x; T y; };
				struct { T u; T v; };
				T elem[2];
			};

			static const Vector2D<T> Zero;
			static const Vector2D<T> Up;
			static const Vector2D<T> Down;
			static const Vector2D<T> Right;
			static const Vector2D<T> Left;
			
			// Constructor
			Vector2D()
				:x(T(0))
				, y(T(0))
			{}

			explicit Vector2D(float _x, float _y)
				:x(_x)
				, y(_y)
			{}

			explicit Vector2D(int _x, int _y)
				:x(_x)
				, y(_y)
			{}

			explicit Vector2D(char _x, char _y)
				:x(_x)
				, y(_y)
			{}

			explicit Vector2D(unsigned char _x, unsigned char _y)
				:x(_x)
				, y(_y)
			{}

			Vector2D(const Vector2D& v) = default;


			T*					GetData() { return &x; }
			const float			Length() const { return std::sqrtf(x * x + y * y); }
			const Vector2D<T>	Normalize() { return Vector2D<T>(x / Length(), y / Length()); }
			const Vector3D<T>	ToAffine() const { return Vector3D<T>(x, y, T(1)); }

			// Operator Overloading
			const Vector2D<T>	operator+() const { return *this; }
			const Vector2D<T>	operator-() const { return Vector2D<T>(-x, -y);  }

			Vector2D<T>&		operator=(Vector2D<T>&& v) = default;
			Vector2D<T>&		operator=(const Vector2D<T>& v) { x = v.x; y = v.y; return *this; }

			const Vector2D<T>	operator+(const Vector2D<T>& v) const { return Vector2D<T>{x + v.x, y + v.y}; }
			const Vector2D<T>	operator-(const Vector2D<T>& v) const { return Vector2D<T>{x - v.x, y - v.y}; }
			const Vector2D<T>	operator*(T scalar) const { return Vector2D<T>{x * scalar, y * scalar }; }
			const Vector2D<T>	operator/(T scalar) const { KEPLER_ASSERT(scalar != 0, "Zero devided"); return Vector2D<T>{x / scalar, y / scalar}; }

			Vector2D<T>&		operator+=(const Vector2D<T>& v) { x += v.x; y += v.y; return *this; }
			Vector2D<T>&		operator*=(const T scalar) { x *= scalar; y *= scalar; return *this; }
			Vector2D<T>&		operator-=(const Vector2D<T>& v) { x -= v.x; y -= v.y; return *this; }
			Vector2D<T>&		operator/=(const T scalar) { x /= scalar; y /= scalar; return *this; }

			T& operator[](const int index) { KEPLER_ASSERT(index >= 0 && index < 2, "Index Out of range"); return elem[index]; }

			// Friend operator overload functions
			friend const bool			operator==(const Vector2D<T>& l, const Vector2D<T>& r) { return ((l.x == r.x) && (l.y == r.y)); }
			friend const bool			operator!=(const Vector2D<T>& l, const Vector2D<T>& r) { return ((l.x != r.x) || (l.y != r.y)); }
			friend const Vector2D<T>	operator*(const T scalar, const Vector2D<T>& v) { return Vector2D<T>{scalar* v.x, scalar* v.y}; }
		};

		template <typename T>
		inline const T Dot(const Vector2D<T>& lv, const Vector2D<T>& rv) { return lv.x * rv.x + lv.y * rv.y; }

		template <typename T> const Vector2D<T> Vector2D<T>::Zero	= Vector2D<T>(0, 0);
		template <typename T> const Vector2D<T> Vector2D<T>::Up		= Vector2D<T>(1, 0);
		template <typename T> const Vector2D<T> Vector2D<T>::Down	= Vector2D<T>(-1, 0);
		template <typename T> const Vector2D<T> Vector2D<T>::Right	= Vector2D<T>(0, 1);
		template <typename T> const Vector2D<T> Vector2D<T>::Left	= Vector2D<T>(0, -1);
	}
}