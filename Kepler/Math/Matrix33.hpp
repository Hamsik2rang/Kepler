#pragma once

#include "Intrinsic.h"
#include "Vector3.hpp"

namespace kepler {
	
	struct Matrix33
	{
		union
		{
			Vector3 row[3];
			float elem[9];
		};

		// Static Variables --------------------------------------
		static const Matrix33 Zero;
		static const Matrix33 Identity;
		//--------------------------------------------------------

		// Constructor -------------------------------------------
		Matrix33()
			: elem{ 0.0f }
		{}

		Matrix33(const float _elem[9])
			: elem{ _elem[0], _elem[1],_elem[2],
					_elem[3], _elem[4], _elem[5],
					_elem[6], _elem[7], _elem[8]}
		{}

		Matrix33(const float e00, const float e01, const float e02,
				 const float e10, const float e11, const float e12,
				 const float e20, const float e21, const float e22)
			: elem{ e00, e01, e02,
					e10,e11,e12,
					e20,e21,e22 }
		{}

		Matrix33(const Vector3& row0, const Vector3& row1, const Vector3& row2)
			: row{ row0, row1, row2 }
		{}
		//--------------------------------------------------------
		
		// Member Functions --------------------------------------
		__forceinline const Matrix33 Transpose() const
		{
			Matrix33 result{};

			__m128 m0 = row[0].ToM128();
			__m128 m1 = row[1].ToM128();
			__m128 m2 = row[2].ToM128();

			__m128 x0011 = _mm_shuffle_ps(m0, m1, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 x2222 = _mm_shuffle_ps(m2, m2, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 x0120 = _mm_shuffle_ps(x0011, x2222, _MM_SHUFFLE(0, 0, 2, 0));

			__m128 y0011 = _mm_shuffle_ps(m0, m1, _MM_SHUFFLE(1, 1, 1, 1));
			__m128 y2222 = _mm_shuffle_ps(m2, m2, _MM_SHUFFLE(1, 1, 1, 1));
			__m128 y0120 = _mm_shuffle_ps(y0011, y2222, _MM_SHUFFLE(0, 0, 2, 0));

			__m128 z0011 = _mm_shuffle_ps(m0, m1, _MM_SHUFFLE(2, 2, 2, 2));
			__m128 z2222 = _mm_shuffle_ps(m2, m2, _MM_SHUFFLE(2, 2, 2, 2));
			__m128 z0120 = _mm_shuffle_ps(z0011, z2222, _MM_SHUFFLE(0, 0, 2, 0));

			StoreM128ToVec3(result.row[0], x0120);
			StoreM128ToVec3(result.row[1], y0120);
			StoreM128ToVec3(result.row[2], z0120);

			return result;
		}
		//--------------------------------------------------------

		// Operator Overloadings ---------------------------------
		__forceinline Matrix33& operator=(const Matrix33& rhs)
		{
			row[0] = rhs.row[0];
			row[1] = rhs.row[1];
			row[2] = rhs.row[2];

			return *this;
		}

		__forceinline const Matrix33 operator+() const
		{ 
			return *this;
		}

		__forceinline const Matrix33 operator-() const
		{
			Matrix33 result{ -row[0], -row[1], -row[2] };

			return *this;
		}

		__forceinline const Matrix33 operator+(const Matrix33& rhs) const
		{
			Matrix33 result{
				row[0] + rhs.row[0],
				row[1] + rhs.row[1],
				row[2] + rhs.row[2],
			};

			return result;
		}

		__forceinline const Matrix33 operator-(const Matrix33& rhs) const
		{
			Matrix33 result{
				row[0] - rhs.row[0],
				row[1] - rhs.row[1],
				row[2] - rhs.row[2],
			};

			return result;
		}

		__forceinline const Matrix33 operator*(const Matrix33& rhs) const
		{
			Matrix33 result{};

			for (int i = 0; i < 3; i++)
			{
				__m128 xyz0 = row[i].ToM128();
				
				__m128 xxxx = _mm_shuffle_ps(xyz0, xyz0, _MM_SHUFFLE(0, 0, 0, 0));
				__m128 yyyy = _mm_shuffle_ps(xyz0, xyz0, _MM_SHUFFLE(1, 1, 1, 1));
				__m128 zzzz = _mm_shuffle_ps(xyz0, xyz0, _MM_SHUFFLE(2, 2, 2, 2));

				__m128 e0 = _mm_mul_ps(xxxx, rhs.row[0].ToM128());
				__m128 e1 = _mm_mul_ps(yyyy, rhs.row[1].ToM128());
				__m128 e2 = _mm_mul_ps(zzzz, rhs.row[2].ToM128());

				StoreM128ToVec3(result.row[i], _mm_add_ps(e0, _mm_add_ps(e1, e2)));
			}

			return result;
		}

		__forceinline const Matrix33 operator*(const float rhs) const
		{
			Matrix33 result{
				row[0] * rhs,
				row[1] * rhs,
				row[2] * rhs,
			};

			return result;
		}

		__forceinline const Matrix33 operator/(const Matrix33& rhs) const
		{
			Matrix33 result{
				row[0] / rhs.row[0],
				row[1] / rhs.row[1],
				row[2] / rhs.row[2],
			};

			return result;
		}

		__forceinline const Matrix33 operator/(const float rhs) const
		{
			Matrix33 result{
				row[0] / rhs,
				row[1] / rhs,
				row[2] / rhs
			};

			return result;
		}

		__forceinline Matrix33& operator+=(const Matrix33& rhs)
		{
			row[0] += rhs.row[0];
			row[1] += rhs.row[1];
			row[2] += rhs.row[2];

			return *this;
		}

		__forceinline Matrix33& operator-=(const Matrix33& rhs)
		{
			row[0] -= rhs.row[0];
			row[1] -= rhs.row[1];
			row[2] -= rhs.row[2];

			return *this;
		}

		__forceinline Matrix33& operator*=(const Matrix33& rhs)
		{
			for (int i = 0; i < 3; i++)
			{
				__m128 xyz0 = row[i].ToM128();

				__m128 xxxx = _mm_shuffle_ps(xyz0, xyz0, _MM_SHUFFLE(0, 0, 0, 0));
				__m128 yyyy = _mm_shuffle_ps(xyz0, xyz0, _MM_SHUFFLE(1, 1, 1, 1));
				__m128 zzzz = _mm_shuffle_ps(xyz0, xyz0, _MM_SHUFFLE(2, 2, 2, 2));

				__m128 e0 = _mm_mul_ps(xxxx, rhs.row[0].ToM128());
				__m128 e1 = _mm_mul_ps(yyyy, rhs.row[1].ToM128());
				__m128 e2 = _mm_mul_ps(zzzz, rhs.row[2].ToM128());

				StoreM128ToVec3(row[i], _mm_add_ps(e0, _mm_add_ps(e1, e2)));
			}

			return *this;
		}

		__forceinline Matrix33& operator/=(const Matrix33& rhs)
		{
			row[0] /= rhs.row[0];
			row[1] /= rhs.row[1];
			row[2] /= rhs.row[2];
		
			return *this;
		}

		__forceinline Matrix33& operator/=(const float rhs)
		{
			row[0] /= rhs;
			row[1] /= rhs;
			row[2] /= rhs;

			return *this;
		}
		//--------------------------------------------------------

		// Friend Operator Overloadings --------------------------
		__forceinline friend const Matrix33 operator*(const float lhs, const Matrix33& rhs)
		{
			Matrix33 result{
				lhs * rhs.row[0],
				lhs * rhs.row[1],
				lhs * rhs.row[2]
			};

			return result;
		}

		__forceinline friend const bool operator==(const Matrix33& lhs, const Matrix33& rhs)
		{
			return (lhs.row[0] == rhs.row[0] &&
					lhs.row[1] == rhs.row[1] &&
					lhs.row[2] == rhs.row[2]);
		}

		__forceinline friend const bool operator!=(const Matrix33& lhs, const Matrix33& rhs)
		{
			return !(lhs == rhs);
		}
		//--------------------------------------------------------
	};

	using Mat33f = Matrix33;
}