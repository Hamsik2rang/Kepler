#pragma once

#include <initializer_list>
#include <cstring>

#include "Vector.h"

namespace kepler {
	namespace math {

		template <typename T> struct Matrix22;
		template <typename T> struct Matrix33;
		template <typename T> struct Matrix44;

		using Mat22f = Matrix22<float>;
		using Mat22i = Matrix22<int>;
		using Mat22c = Matrix22<char>;

		template <typename T>
		struct Matrix22
		{
			union
			{
				struct { Vector2D<T> v0; Vector2D<T> v1; };
				struct { T r0[2]; T r1[2]; };
				struct { T r[2][2]; };
				T elem[4];
			};

			static const Matrix22<T> Identity;
			static const Matrix22<T> Zero;

			Matrix22()
				:elem{ T(0), T(0), T(0), T(0) }
			{}

			Matrix22(Vector2D<T> _v0, Vector2D<T> _v1)
				:v0(_v0)
				,v1(_v1)
			{}

			Matrix22(const T(&_elem)[4])
				:elem{
				_elem[0], _elem[1], 
				_elem[2], _elem[3]}
			{}

			Matrix22(const T e00, const T e01, const T e10, const T e11)
				:elem{
				e00, e01,
				e10, e11}
			{}

			Matrix22(const T(&r0)[2], const T(&r1)[2])
				:elem{
				r0[0], r0[1], 
				r1[0], r1[1]}
			{}

			Matrix22(const Matrix22& m) = default;
			Matrix22(Matrix22&& m) = default;

			T* GetData() { return elem; }
			const Matrix22<T> Transpose() const { return Matrix22<T>(r0[0], r1[0], r0[1], r1[1]); }

			// Operator Overloading
			const Matrix22<T>	operator+() const { return *this; }
			const Matrix22<T>	operator-() const { return Matrix22<T>(-elem[0], -elem[1], -elem[2], -elem[3]); }

			Matrix22<T>&		operator=(const Matrix22<T>& m) { ::memcpy_s(elem, sizeof(T) * 4, m.elem, sizeof(T) * 4); return *this; }
			T*					operator[](int index) { KEPLER_ASSERT(index >= 0 && index < 2, "index out of bound"); return r[index]; }
			
			const Matrix22<T> operator+(const Matrix22<T>& m) const
			{
				return Matrix22<T>{
					r0[0] + m.r0[0], r0[1] + m.r0[1],
					r1[0] + m.r1[0], r1[1] + m.r1[1]
				};
			}

			const Matrix22<T> operator-(const Matrix22<T>& m) const
			{
				return Matrix22<T>{
					r0[0] - m.r0[0], r0[1] - m.r0[1],
					r1[0] - m.r1[0], r1[1] - m.r1[1]
				};
			}

			const Matrix22<T> operator*(const Matrix22<T>& m) const
			{
				Matrix22<T> temp = m.Transpose();
				return Matrix22<T>{
					Dot(v0, m.v0), Dot(v0, m.v1), 
					Dot(v1, m.v0), Dot(v1, m.v1) };
			}

			const Matrix22<T> operator*(const T t) const
			{ 
				return Matrix22<T>(
					elem[0] * t, elem[1] * t, 
					elem[2] * t, elem[3] * t); 
			}
			
			const Matrix22<T> operator/(const T t) const
			{ 
				return Matrix22<T>(
					elem[0] / t, elem[1] / t, 
					elem[2] / t, elem[3] / t);
			}

			Matrix22<T>& operator+=(const Matrix22<T>& m) 
			{
				elem[0] += m.elem[0];
				elem[1] += m.elem[1]; 
				elem[2] += m.elem[2]; 
				elem[3] += m.elem[3]; 
				
				return *this; 
			}
			
			Matrix22<T>& operator-=(const Matrix22<T>& m)
			{ 
				elem[0] -= m.elem[0]; 
				elem[1] -= m.elem[1]; 
				elem[2] -= m.elem[2]; 
				elem[3] -= m.elem[3]; 
				
				return *this; 
			}
			
			Matrix22<T>& operator*=(const Matrix22<T>& m) 
			{
				Matrix22<T> tp = m.Transpose();
				elem[0] = Dot(v0, tp.v0);
				elem[1] = Dot(v0, tp.v1);
				elem[2] = Dot(v1, tp.v0);
				elem[3] = Dot(v1, tp.v1);

				return *this;
			}
			
			Matrix22<T>& operator*=(const T t)
			{
				elem[0] *= t;
				elem[1] *= t;
				elem[2] *= t;
				elem[3] *= t;

				return *this;
			}

			Matrix22<T>& operator/=(const T t)
			{
				elem[0] /= t;
				elem[1] /= t;
				elem[2] /= t;
				elem[3] /= t;

				return *this;
			}

			// Friend Operator Overloading
			friend const bool operator==(const Matrix22<T>& lm, const Matrix22<T>& rm) { return (lm.elem[0] == rm.elem[0] && lm.elem[1] == rm.elem[1] && lm.elem[2] == rm.elem[2] && lm.elem[3] == rm.elem[3]); }
			friend const bool operator!=(const Matrix22<T>& lm, const Matrix22<T>& rm) { return (lm.elem[0] != rm.elem[0] || lm.elem[1] != rm.elem[1] || lm.elem[2] != rm.elem[2] || lm.elem[3] != rm.elem[3]); }
			friend const Matrix22<T> operator*(const T t, const Matrix22<T>& m) { return Matrix22<T>(t * m.elem[0], t * m.elem[1], t * m.elem[2], t * m.elem[3]); }
		};

		template <typename T>
		const Matrix22<T> Matrix22<T>::Identity = Matrix22<T>{
			1.0f, 0.0f,
			0.0f, 1.0f };

		template <typename T>
		const Matrix22<T> Matrix22<T>::Zero = Matrix22<T>{
			0.0f, 0.0f,
			0.0f, 0.0f };
	}
}