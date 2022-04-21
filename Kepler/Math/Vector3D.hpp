#pragma once

#include <cmath>
#include <type_traits>

#include "Core/Base.h"

namespace kepler {

	namespace math {
		template <typename T> struct Vector2D;
		template <typename T> struct Vector3D;
		template <typename T> struct Vector4D;

		using Vec3f = Vector3D<float>;
		using Vec3i = Vector3D<int>;
		using Vec3c = Vector3D<char>;
		using Vec3b = Vector3D<unsigned char>;

		template <typename T>
		struct /*alignas(16)*/ Vector3D
		{
			union
			{
				struct { T x; T y; T z; };
				struct { T r; T g; T b; };
				T elem[3];
			};

			static const Vector3D<T> Zero;
			static const Vector3D<T> Up;
			static const Vector3D<T> Down;
			static const Vector3D<T> Right;
			static const Vector3D<T> Left;
			static const Vector3D<T> Front;
			static const Vector3D<T> Back;

			Vector3D()
				:x(T(0))
				,y(T(0))
				,z(T(0))
			{}

			explicit Vector3D(float _x, float _y, float _z)
				: x(_x)
				,y(_y)
				,z(_z)
			{}

			explicit Vector3D(int _x, int _y, int _z)
				:x(_x)
				,y(_y)
				,z(_z)
			{}

			explicit Vector3D(char _x, char _y, char _z)
				:x(_x)
				,y(_y)
				,z(_z)
			{}

			explicit Vector3D(unsigned char _x, unsigned char _y, unsigned char _z)
				:x(_x)
				,y(_y)
				,z(_z)
			{}
			
			Vector3D(const Vector3D& v) = default;
			
			T*					GetData() { return &x; }
			const float			Length() const { return std::sqrtf(x * x + y * y + z * z); }
			const Vector3D<T>	Normalize() const { return Vector3D<T>(x / Length(), y / Length(), z / Length()); }
			const Vector2D<T>	ToCartesian() const { return Vector2D<T>(x / z, y / z);  }
			const Vector4D<T>	ToAffine() const { return Vector4D<T>(x, y, z, 1); }
			
			// Operator Overloading
			const Vector3D<T>	operator+() const { return *this; }
			const Vector3D<T>	operator-() const { Vector3D<T> ret(T(-1) * x, T(-1) * y, T(-1) * z); return ret; }

			Vector3D<T>&		operator=(Vector3D<T>&& v) = default;
			Vector3D<T>&		operator=(const Vector3D<T>& v) { x = v.x; y = v.y; z = v.z; return *this; }
			const Vector3D<T>	operator+(const Vector3D<T>& v) const { return Vector3D<T>(x + v.x, y + v.y, z + v.z); }
			const Vector3D<T>	operator-(const Vector3D<T>& v) const { return Vector3D<T>(x - v.x, y - v.y, z - v.z); }
			const Vector3D<T>	operator*(const T scalar) const { return Vector3D<T>(x * scalar, y * scalar, z * scalar); }
			const Vector3D<T>	operator/(const T scalar) const { return Vector3D<T>(x / scalar, y / scalar, z / scalar); }

			Vector3D<T>&		operator+=(const Vector3D<T>& v) { x += v.x; y += v.y; z += v.z; return *this; }
			Vector3D<T>&		operator-=(const Vector3D<T>& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
			Vector3D<T>&		operator*=(const T scalar) const { x *= scalar; y *= scalar; z *= scalar; return *this; }
			Vector3D<T>&		operator/=(const T scalar) const { x /= scalar; y /= scalar; z /= scalar; return *this; }

			T&					operator[](int index) { return elem[index]; }

			// Friend Operator Overloading
			friend const Vector3D<T>	operator*(const T scalar, const Vector3D<T>& v) { return Vector3D<T>(scalar * v.x, scalar * v.y, scalar * v.z); }
			friend const bool			operator==(const Vector3D<T>& lv, const Vector3D<T>& rv) { return ((lv.x == rv.x) && (lv.y == rv.y) && (lv.z == rv.z)); }
			friend const bool			operator!=(const Vector3D<T>& lv, const Vector3D<T>& rv) { return ((lv.x != rv.x) || (lv.y != rv.y) || (lv.z != rv.z)); }
		};

		template <typename T>
		inline const T Dot(const Vector3D<T>& lv, const Vector3D<T>& rv) 
		{ 
			return lv.x * rv.x + lv.y * rv.y + lv.z * rv.z;
		}

		template <typename T>
		inline const Vector3D<T> Cross(const Vector3D<T>& lv, const Vector3D<T>& rv)
		{
			return Vector3D<T>(lv.y * rv.z - lv.z * rv.y, lv.z * rv.x - lv.x * rv.z, lv.x * rv.y - lv.y * rv.x);
		}

		template <typename T> const Vector3D<T> Vector3D<T>::Zero	= Vector3D<T>(0, 0, 0);
		template <typename T> const Vector3D<T> Vector3D<T>::Up		= Vector3D<T>(0, 1, 0);
		template <typename T> const Vector3D<T> Vector3D<T>::Down	= Vector3D<T>(0, -1, 0);
		template <typename T> const Vector3D<T> Vector3D<T>::Right	= Vector3D<T>(1, 0, 0);
		template <typename T> const Vector3D<T> Vector3D<T>::Left	= Vector3D<T>(-1, 0, 0);
		template <typename T> const Vector3D<T> Vector3D<T>::Front	= Vector3D<T>(0, 0, 1);
		template <typename T> const Vector3D<T> Vector3D<T>::Back	= Vector3D<T>(0, 0, -1);
	}
}