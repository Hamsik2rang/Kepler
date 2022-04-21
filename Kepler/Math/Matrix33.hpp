#pragma once

#include <initializer_list>
#include <cstring>

#include "Vector.h"

namespace kepler {
	namespace math {

		template <typename T> struct Matrix22;
		template <typename T> struct Matrix33;
		template <typename T> struct Matrix44;

		using Mat33f = Matrix33<float>;
		using Mat33i = Matrix33<int>;
		using Mat33c = Matrix33<char>;

		template <typename T>
		struct Matrix33
		{
			union
			{
				struct { Vector3D<T> v0; Vector3D<T> v1; Vector3D<T> v2; };
				struct { T r0[3]; T r1[3]; T r2[3]; };
				struct { T r[3][3]; };
				T elem[9];
			};

			static const Matrix33<T> Zero;
			static const Matrix33<T> Identity;

			Matrix33()
				:elem{ T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0)}
			{}

			Matrix33(const T e00, const T e01, const T e02, const T e10, const T e11, const T e12, const T e20, const T e21, const T e22)
				:elem{
				e00, e01, e02,
				e10, e11, e12,
				e20, e21, e22 }
			{}

			Matrix33(const T(&_elem)[9])
				:elem{
				_elem[0], _elem[1], _elem[2],
				_elem[3], _elem[4], _elem[5],
				_elem[6], _elem[7], _elem[8] }
			{}

			Matrix33(const T(&r0)[3], const T(&r1)[3], const T(&r2)[3])
				:elem{
				r0[0], r0[1], r0[2],
				r1[0], r1[1], r1[2],
				r2[0], r2[1], r2[2] }
			{}

			Matrix33(const Matrix33& m) = default;
			Matrix33(Matrix33&& m) = default;

			T* GetData() { return elem; }
			const Matrix33<T> Transpose() const 
			{ 
				return Matrix33<T>(
					r0[0], r1[0], r2[0], 
					r0[1], r1[1], r2[1],
					r0[2], r1[2], r2[2]); 
			}

			// Operator Overloading
			T*					operator[](int index) { KEPLER_ASSERT(index >= 0 && index < 3, "index out of bound"); return r[index]; }
			const Matrix33<T>	operator+() const { return *this; }
			const Matrix33<T>	operator-() const 
			{ 
				return Matrix33<T>(
					-elem[0], -elem[1], -elem[2],
					-elem[3], -elem[4], -elem[5],
					-elem[6], -elem[7], -elem[8]); 
			}

			Matrix33<T>& operator=(const Matrix33<T>& m) 
			{ 
				elem[0] = m.elem[0]; 
				elem[1] = m.elem[1]; 
				elem[2] = m.elem[2]; 
				elem[3] = m.elem[3]; 
				elem[4] = m.elem[4];
				elem[5] = m.elem[5]; 
				elem[6] = m.elem[6]; 
				elem[7] = m.elem[7]; 
				elem[8] = m.elem[8]; 
				return *this; 
			}

			const Matrix33<T> operator+(const Matrix33<T>& m) const
			{
				return Matrix33<T>(
					elem[0] + m.elem[0], elem[1] + m.elem[1], elem[2] + m.elem[2],
					elem[3] + m.elem[3], elem[4] + m.elem[4], elem[5] + m.elem[5],
					elem[6] + m.elem[6], elem[7] + m.elem[7], elem[8] + m.elem[8]
					);
			}

			const Matrix33<T> operator-(const Matrix33<T>& m) const
			{
				return Matrix33<T>(
					elem[0] - m.elem[0], elem[1] - m.elem[1], elem[2] - m.elem[2],
					elem[3] - m.elem[3], elem[4] - m.elem[4], elem[5] - m.elem[5],
					elem[6] - m.elem[6], elem[7] - m.elem[7], elem[8] - m.elem[8]
					);
			}
			
			const Matrix33<T> operator*(const Matrix33<T>& m) const
			{
				Matrix33<T> tp = m.Transpose();
				return Matrix33<T>(
					Dot(v0, tp.v0), Dot(v0, tp.v1), Dot(v0, tp.v2),
					Dot(v1, tp.v0), Dot(v1, tp.v1), Dot(v1, tp.v2),
					Dot(v2, tp.v0), Dot(v2, tp.v1), Dot(v2, tp.v2)
					);
			}

			const Matrix33<T> operator*(const T t) const
			{
				return Matrix33<T>(
					elem[0] * t, elem[1] * t, elem[2] * t,
					elem[3] * t, elem[4] * t, elem[5] * t,
					elem[6] * t, elem[7] * t, elem[8] * t
					);
			}

			const Matrix33<T> operator/(const T t) const
			{
				return Matrix33<T>(
					elem[0] / t, elem[1] / t, elem[2] / t,
					elem[3] / t, elem[4] / t, elem[5] / t,
					elem[6] / t, elem[7] / t, elem[8] / t
					);
			}

			Matrix33<T>& operator+=(const Matrix33<T>& m)
			{
				elem[0] += m.elem[0];
				elem[1] += m.elem[1];
				elem[2] += m.elem[2];
				elem[3] += m.elem[3];
				elem[4] += m.elem[4];
				elem[5] += m.elem[5];
				elem[6] += m.elem[6];
				elem[7] += m.elem[7];
				elem[8] += m.elem[8];
				
				return *this;
			}

			Matrix33<T>& operator-=(const Matrix33<T>& m)
			{
				elem[0] -= m.elem[0];
				elem[1] -= m.elem[1];
				elem[2] -= m.elem[2];
				elem[3] -= m.elem[3];
				elem[4] -= m.elem[4];
				elem[5] -= m.elem[5];
				elem[6] -= m.elem[6];
				elem[7] -= m.elem[7];
				elem[8] -= m.elem[8];

				return *this;
			}

			Matrix33<T>& operator*=(const Matrix33<T>& m)
			{
				Matrix33<T> tp = m.Transpose();
				elem[0] = Dot(v0, tp.v0);
				elem[1] = Dot(v0, tp.v1);
				elem[2] = Dot(v0, tp.v2);
				elem[3] = Dot(v1, tp.v0);
				elem[4] = Dot(v1, tp.v1);
				elem[5] = Dot(v1, tp.v2);
				elem[6] = Dot(v2, tp.v0);
				elem[7] = Dot(v2, tp.v1);
				elem[8] = Dot(v2, tp.v2);

				return *this;
			}

			Matrix33<T>& operator*=(const T t)
			{
				elem[0] *= t;
				elem[1] *= t;
				elem[2] *= t;
				elem[3] *= t;
				elem[4] *= t;
				elem[5] *= t;
				elem[6] *= t;
				elem[7] *= t;
				elem[8] *= t;

				return *this;
			}

			Matrix33<T>& operator/=(const T t)
			{
				elem[0] /= t;
				elem[1] /= t;
				elem[2] /= t;
				elem[3] /= t;
				elem[4] /= t;
				elem[5] /= t;
				elem[6] /= t;
				elem[7] /= t;
				elem[8] /= t;

				return *this;
			}

			// Friend Operator Overloading
			friend const Matrix33<T> operator*(const T t, const Matrix33<T>& m)
			{
				return Matrix33<T>(
					m.elem[0] * t, m.elem[1] * t, m.elem[2] * t,
					m.elem[3] * t, m.elem[4] * t, m.elem[5] * t,
					m.elem[6] * t, m.elem[7] * t, m.elem[8] * t
					);
			}

			friend const bool operator==(const Matrix33<T>& lm, const Matrix33<T>& rm)
			{
				return (lm.elem[0] == rm.elem[0] && lm.elem[1] == rm.elem[1] && lm.elem[2] == rm.elem[2]
					 && lm.elem[3] == rm.elem[3] && lm.elem[4] == rm.elem[4] && lm.elem[5] == rm.elem[5]
					 && lm.elem[6] == rm.elem[6] && lm.elem[7] == rm.elem[7] && lm.elem[8] == rm.elem[8]);
			}

			friend const bool operator!=(const Matrix33<T>& lm, const Matrix33<T>& rm)
			{
				return (lm.elem[0] != rm.elem[0] || lm.elem[1] != rm.elem[1] || lm.elem[2] != rm.elem[2]
					 || lm.elem[3] != rm.elem[3] || lm.elem[4] != rm.elem[4] || lm.elem[5] != rm.elem[5]
					 || lm.elem[6] != rm.elem[6] || lm.elem[7] != rm.elem[7] || lm.elem[8] != rm.elem[8]);
			}
		};

		template <typename T>
		const Matrix33<T> Matrix33<T>::Zero{
			T(0), T(0), T(0),
			T(0), T(0), T(0),
			T(0), T(0), T(0) };

		template <typename T>
		const Matrix33<T> Matrix33<T>::Identity{
			T(1), T(0), T(0),
			T(0), T(1), T(0),
			T(0), T(0), T(1)
		};
	}
}