#pragma once

// for SIMD, not necessary now
//#include <intrin.h>

#include <type_traits>
#include <cmath>

#include "Core/Base.h"

namespace kepler {
	namespace math {
		// float, int, char만을 받도록 template 인자를 제한합니다.
#define TYPE_CONSTRAINT_FLOAT_INT_CHAR_UCHAR(T) template <typename T, typename =\
			std::enable_if<\
			std::is_same<float, T>::value || std::is_same<int, T>::value || std::is_same<char, T>::value || std::is_same<unsigned char, T>::value>>

		TYPE_CONSTRAINT_FLOAT_INT_CHAR_UCHAR(T)
		struct /*alignas(16)*/ Vector2D
		{
			union
			{
				struct { T x; T y; };
				struct { T u; T v; };
				T elem[2];
			};

			// ctor
			explicit Vector2D()
				:x(T(0)), y(T(0))
			{}
			explicit Vector2D(float _x, float _y) 
				:x(_x), y(_y)
			{}
			explicit Vector2D(int _x, int _y)
				:x(_x), y(_y)
			{}
			explicit Vector2D(char _x, char _y)
				:x(_x), y(_y)
			{}
			explicit Vector2D(unsigned char _x, unsigned char _y)
				:x(_x), y(_y)
			{}


			static const Vector2D<T> Zero() { return Vector2D<T>{static_cast<T>(0), static_cast<T>(0) }; }
			T* GetData() { return &x; }
			const float GetLength() const { return std::sqrtf(x * x + y * y); }

			// Operation Overloading
			Vector2D<T>&		operator=(const Vector2D<T>& v) { x = v.x; y = v.y; return *this; }
			TYPE_CONSTRAINT_FLOAT_INT_CHAR_UCHAR(F)
			Vector2D<T>&		operator=(const Vector2D<F>& v) { x = v.x; y = v.y; return *this; }

			const Vector2D<T>	operator+(const Vector2D<T>& v) const { return Vector2D<T>{x + v.x, y + v.y}; }
			const Vector2D<T>	operator-(const Vector2D<T>& v) const { return Vector2D<T>{x - v.x, y - v.y}; }
			const Vector2D<T>	operator*(T scalar) const { return Vector2D<T>{x * scalar, y * scalar }; }
			const Vector2D<T>	operator/(T scalar) const { KEPLER_ASSERT(scalar != 0); return Vector2D<T>{x / scalar, y / scalar}; }

			Vector2D<T>&		operator+=(const Vector2D<T>& v) { x += v.x; y += v.y; return *this; }
			Vector2D<T>&		operator-=(const Vector2D<T>& v) { x -= v.x; y -= v.y; return *this; }
			Vector2D<T>&		operator*=(const T scalar) { x *= T; y *= T; return *this; }

			T& operator[](const int index) { KEPLER_ASSERT(index >= 0 && index < 2); return elem[index]; }

			// Friend operation overload functions
			friend const bool			operator==(const Vector2D<T>& l, const Vector2D<T>& r) { return ((l.x == r.x) && (l.y == r.y)); }
			friend const bool			operator!=(const Vector2D<T>& l, const Vector2D<T>& r) { return ((l.x != r.x) || (l.y != r.y)); }
			friend const Vector2D<T>	operator*(const T scalar, const Vector2D<T>& v) { KEPLER_ASSERT(scalar != T(0)); return Vector2D<T>{scalar* v.x, scalar* v.y}; }
		};

		//TYPE_CONSTRAINT_FLOAT_INT_CHAR_UCHAR(T)
		//inline const Vector2D<T> operator*(const T scalar, const Vector2D<T>& v) { KEPLER_ASSERT(scalar != T(0)); return Vector2D<T>{scalar* v.x, scalar* v.y}; }
		//
		//TYPE_CONSTRAINT_FLOAT_INT_CHAR_UCHAR(T)
		//inline const bool operator==(const Vector2D<T>& l, const Vector2D<T>& r) { return ((l.x == r.x) && (l.y == r.y)); }
		//
		//// TODO: 연산 모호성 원인 파악하기. ERROR C2593:'operator !=' is ambiguous
		//   --SOLVED: friend함수를 안에 선언하고 밖에 정의하면 안됩니다.
		//TYPE_CONSTRAINT_FLOAT_INT_CHAR_UCHAR(T)
		//inline const bool operator!=(const Vector2D<T>& l, const Vector2D<T>& r) { return ((l.x != r.x) || (l.y != r.y)); }

		TYPE_CONSTRAINT_FLOAT_INT_CHAR_UCHAR(T)
		inline const T Dot(const Vector2D<T>& lv, const Vector2D<T>& rv) { return lv.x * rv.x + lv.y * rv.y; }

		using Vec2f = Vector2D<float>;			// Vector2D of float
		using Vec2i = Vector2D<int>;			// Vector2D of int
		using Vec2c = Vector2D<char>;			// Vector2D of char
		using Vec2b = Vector2D<unsigned char>;	// Vector2D of byte(uchar)
	}
}