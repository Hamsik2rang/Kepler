#pragma once

#include <cmath>

#include "Core/Base.h"

namespace kepler {
	namespace math {
		template <typename T> struct Vector2D;
		template <typename T> struct Vector3D;
		template <typename T> struct Vector4D;

		using Vec4f = Vector4D<float>;
		using Vec4i = Vector4D<int>;
		using Vec4c = Vector4D<char>;
		using Vec4b = Vector4D<unsigned char>;

		template <typename T>
		struct Vector4D
		{
			union
			{
				struct { T x; T y; T z; T w; };
				struct { T r; T g; T b; T a; };
				T elem[4];
			};

			static const Vector4D<T> Zero;
			static const Vector4D<T> Up;
			static const Vector4D<T> Down;
			static const Vector4D<T> Right;
			static const Vector4D<T> Left;
			static const Vector4D<T> Front;
			static const Vector4D<T> Back;

			// Constructor
			explicit Vector4D(float _x, float _y, float _z, float _a)
				:x(_x)
				,y(_y)
				,z(_z)
				,a(_a)
			{}

			explicit Vector4D(int _x, int _y, int _z, int _a)
				:x(_x)
				,y(_y)
				,z(_z)
				,a(_a)
			{}

			explicit Vector4D(char _x, char _y, char _z, char _a)
				:x(_x)
				,y(_y)
				,z(_z)
				,a(_a)
			{}

			explicit Vector4D(unsigned char _x, unsigned char _y, unsigned char _z, unsigned char _a)
				:x(_x)
				,y(_y)
				,z(_z)
				,a(_a)
			{}

			Vector4D(const Vector4D& v) = default;
			Vector4D(Vector4D&& v) = default;

			T*					GetData() { return &x; }
			const float			Length() const { return ::sqrtf(x * x + y * y + z * z); }
			const Vector3D<T>	ToCartesian() const { return Vector3D<T>(x / w, y / w, z / w); }
			const Vector4D<T>	Normalize() const { return Vector4D<T>(x / Length(), y / Length(), z / Length(), w / Length()); }

			// Operator Overloading
			const Vector4D<T>	operator+() const { return *this; }
			const Vector4D<T>	operator-() const { return Vector4D<T>(-x, -y, -z, -w); }

			Vector4D<T>&		operator=(Vector4D<T>&& v) = default;
			Vector4D<T>&		operator=(const Vector4D<T>& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
			const Vector4D<T>	operator+(const Vector4D<T>& v) const { return Vector4D<T>(x + v.x, y + v.y, z + v.z, w + v.w); }
			const Vector4D<T>	operator-(const Vector4D<T>& v) const { return Vector4D<T>(x - v.x, y - v.y, z - v.z, w - v.w); }
			const Vector4D<T>	operator*(const T scalar) const { return Vector4D<T>(x * scalar, y * scalar, z * scalar, w * scalar); }
			const Vector4D<T>	operator/(const T saclar) const { return Vector4D<T>(x / scalar, y / scalar, z / scalar, w / scalar); }

			Vector4D<T>&		operator+=(const Vector4D<T>& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
			Vector4D<T>&		operator-=(const Vector4D<T>& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
			Vector4D<T>&		operator*=(const T scalar) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
			Vector4D<T>&		operator/=(const T scalar) { x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this; }

			T&					operator[](int index) { KEPLER_ASSERT(index >= 0 && index < 4, "index out of bound"); return elem[index]; }

			// Friend Operator Overloading
			friend const Vector4D<T> operator*(const T scalar, const Vector4D<T>& v) { return Vector4D<T>(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar); }
			friend const bool operator==(const Vector4D<T>& lv, const Vector4D<T>& rv) { return lv.x == rv.x && lv.y == rv.y && lv.z == rv.z && lv.w == rv.w; }
			friend const bool operator!=(const Vector4D<T>& lv, const Vector4D<T>& rv) { return lv.x != rv.x || lv.y != rv.y || lv.z != rv.z || lv.w != rv.w; }
		};

		template <typename T>
		const T Dot(const Vector4D<T>& lv, const Vector4D<T>& rv) { return lv.x * rv.x + lv.y * rv.y + lv.z * rv.z + lv.w * rv.w; }
		

		template <typename T> const Vector4D<T> Vector4D<T>::Zero	= Vector4D<T>(0, 0, 0, 0);
		template <typename T> const Vector4D<T> Vector4D<T>::Up		= Vector4D<T>(0, 1, 0, 0);
		template <typename T> const Vector4D<T> Vector4D<T>::Down	= Vector4D<T>(0, -1, 0, 0);
		template <typename T> const Vector4D<T> Vector4D<T>::Right	= Vector4D<T>(1, 0, 0, 0);
		template <typename T> const Vector4D<T> Vector4D<T>::Left	= Vector4D<T>(-1, 0, 0, 0);
		template <typename T> const Vector4D<T> Vector4D<T>::Front	= Vector4D<T>(0, 0, 1, 0);
		template <typename T> const Vector4D<T> Vector4D<T>::Back	= Vector4D<T>(0, 0, -1, 0);

	}
}