#pragma once

#include <iostream>
#include <stdint.h>

#include "Intrinsic.h"
#include "Vector4.hpp"


namespace kepler {
	// row-majer matrix
	struct alignas(16) Matrix44
	{
		union
		{
			Vector4 row[4];
			float elem[16];
		};
		// Static Variables --------------------------------------
		static const Matrix44 Zero;
		static const Matrix44 Identity;
		//--------------------------------------------------------
		
		// Constructor -------------------------------------------
		Matrix44()
			: elem{ 0.0f }
		{}

		Matrix44(const float _elem[16])
			: elem{ _elem[0],  _elem[1],  _elem[2],  _elem[3],
					_elem[4],  _elem[5],  _elem[6],  _elem[7],
					_elem[8],  _elem[9],  _elem[10], _elem[11],
					_elem[12], _elem[13], _elem[14], _elem[15] }
		{}

		Matrix44(const float e00, const float e01, const float e02, const float e03,
				 const float e10, const float e11, const float e12, const float e13,
				 const float e20, const float e21, const float e22, const float e23,
				 const float e30, const float e31, const float e32, const float e33)
			: elem{ e00, e01, e02, e03,
					e10, e11, e12, e13,
					e20, e21, e22, e23,
					e30, e31, e32, e33 }
		{}

		Matrix44(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3)
			:row{ row0, row1, row2, row3 }
		{}

		Matrix44(const Matrix44& mat) = default;
		Matrix44(Matrix44&& mat) = default;
		//--------------------------------------------------------

		// Member Functions --------------------------------------
		__forceinline const Matrix44 Transpose() const
		{
			Matrix44 result{};

			// make x0 x1 x2 x3
			__m128 x0011 = _mm_shuffle_ps(row[0].ps, row[1].ps, KP_MM_SHUFFLE_LE(0, 0, 0, 0));
			__m128 x2233 = _mm_shuffle_ps(row[2].ps, row[3].ps, KP_MM_SHUFFLE_LE(0, 0, 0, 0));
			// Little Endian( 0b 00 11 00 10 -> 0b 10 00 11 00 (stored in Memory) )
			__m128 x0123 = _mm_shuffle_ps(x0011, x2233, KP_MM_SHUFFLE_LE(0, 2, 0, 2));

			result.row[0].ps = x0123;

			// make y0 y1 y2 y3
			__m128 y0011 = _mm_shuffle_ps(row[0].ps, row[1].ps, KP_MM_SHUFFLE_LE(1, 1, 1, 1));
			__m128 y2233 = _mm_shuffle_ps(row[2].ps, row[3].ps, KP_MM_SHUFFLE_LE(1, 1, 1, 1));
			__m128 y0123 = _mm_shuffle_ps(y0011, y2233, KP_MM_SHUFFLE_LE(0, 2, 0, 2));

			result.row[1].ps = y0123;

			// make z0 z1 z2 z3 
			__m128 z0011 = _mm_shuffle_ps(row[0].ps, row[1].ps, KP_MM_SHUFFLE_LE(2, 2, 2, 2));
			__m128 z2233 = _mm_shuffle_ps(row[2].ps, row[3].ps, KP_MM_SHUFFLE_LE(2, 2, 2, 2));
			__m128 z0123 = _mm_shuffle_ps(z0011,z2233, KP_MM_SHUFFLE_LE(0, 2, 0, 2));

			result.row[2].ps = z0123;

			// make w0 w1 w2 w3
			__m128 w0011 = _mm_shuffle_ps(row[0].ps, row[1].ps, KP_MM_SHUFFLE_LE(3, 3, 3, 3));
			__m128 w2233 = _mm_shuffle_ps(row[2].ps, row[3].ps, KP_MM_SHUFFLE_LE(3, 3, 3, 3));
			__m128 w0123 = _mm_shuffle_ps(w0011, w2233, KP_MM_SHUFFLE_LE(0, 2, 0, 2));

			result.row[3].ps = w0123;

			return result;
		};
		//--------------------------------------------------------
		
		// Operator Overloadings ---------------------------------
		__forceinline Matrix44& operator=(const Matrix44& rhs)
		{
			row[0] = rhs.row[0];
			row[1] = rhs.row[1];
			row[2] = rhs.row[2];
			row[3] = rhs.row[3];
		
			return *this;
		}

		__forceinline const Matrix44 operator+() const
		{
			return *this;
		}

		__forceinline const Matrix44 operator-() const
		{
			Matrix44 result = { -row[0], -row[1], -row[2], -row[3] };

			return result;
		}
		
		__forceinline Vector4& operator[](int index)
		{
			return row[index];
		}

		__forceinline const Matrix44 operator+(const Matrix44& rhs) const
		{
			Matrix44 result{
				row[0] + rhs.row[0],
				row[1] + rhs.row[1], 
				row[2] + rhs.row[2],
				row[3] + rhs.row[3]
			};

			return result;
		}

		__forceinline const Matrix44 operator-(const Matrix44& rhs) const
		{
			Matrix44 result{
				row[0] - rhs.row[0],
				row[1] - rhs.row[1],
				row[2] - rhs.row[2],
				row[3] - rhs.row[3]
			};

			return result;
		}

		__forceinline const Matrix44 operator*(const float rhs) const
		{
			Matrix44 result{
				row[0] * rhs,
				row[1] * rhs,
				row[2] * rhs,
				row[3] * rhs
			};

			return result;
		}

		__forceinline const Matrix44 operator*(const Matrix44& rhs) const
		{
			Matrix44 result{};
			for (int i = 0; i < 4; i++)
			{
				__m128 xxxx = _mm_shuffle_ps(row[i].ps, row[i].ps, KP_MM_SHUFFLE_LE(0, 0, 0, 0));
				__m128 yyyy = _mm_shuffle_ps(row[i].ps, row[i].ps, KP_MM_SHUFFLE_LE(1, 1, 1, 1));
				__m128 zzzz = _mm_shuffle_ps(row[i].ps, row[i].ps, KP_MM_SHUFFLE_LE(2, 2, 2, 2));
				__m128 wwww = _mm_shuffle_ps(row[i].ps, row[i].ps, KP_MM_SHUFFLE_LE(3, 3, 3, 3));

				__m128 e0 = _mm_mul_ps(xxxx, rhs.row[0].ps);
				__m128 e1 = _mm_mul_ps(yyyy, rhs.row[1].ps);
				__m128 e2 = _mm_mul_ps(zzzz, rhs.row[2].ps);
				__m128 e3 = _mm_mul_ps(wwww, rhs.row[3].ps);

				__m128 e01 = _mm_add_ps(e0, e1);
				__m128 e02 = _mm_add_ps(e2, e3);

				result.row[i].ps = _mm_add_ps(e01, e02);
			}

			return result;
		}
		
		__forceinline const Matrix44 operator/(const Matrix44& rhs) const
		{
			Matrix44 result{
				row[0] / rhs.row[0],
				row[1] / rhs.row[1],
				row[2] / rhs.row[2],
				row[3] / rhs.row[3]
			};

			return result;
		}

		__forceinline const Matrix44 operator/(const float rhs) const
		{
			Matrix44 result{
				row[0] / rhs,
				row[1] / rhs,
				row[2] / rhs,
				row[3] / rhs
			};

			return result;
		}

		__forceinline Matrix44& operator+=(const Matrix44& rhs)
		{
			row[0] += rhs.row[0];
			row[1] += rhs.row[1];
			row[2] += rhs.row[2];
			row[3] += rhs.row[3];
		
			return *this;
		}

		__forceinline Matrix44& operator-=(const Matrix44& rhs)
		{
			row[0] -= rhs.row[0];
			row[1] -= rhs.row[1];
			row[2] -= rhs.row[2];
			row[3] -= rhs.row[3];

			return *this;
		}

		__forceinline Matrix44& operator*=(const Matrix44& rhs)
		{
			for (int i = 0; i < 4; i++)
			{
				__m128 xxxx = _mm_shuffle_ps(row[0].ps, row[0].ps, KP_MM_SHUFFLE_LE(0, 0, 0, 0));
				__m128 yyyy = _mm_shuffle_ps(row[1].ps, row[1].ps, KP_MM_SHUFFLE_LE(1, 1, 1, 1));
				__m128 zzzz = _mm_shuffle_ps(row[2].ps, row[2].ps, KP_MM_SHUFFLE_LE(2, 2, 2, 2));
				__m128 wwww = _mm_shuffle_ps(row[3].ps, row[3].ps, KP_MM_SHUFFLE_LE(3, 3, 3, 3));

				__m128 e0 = _mm_mul_ps(xxxx, rhs.row[0].ps);
				__m128 e1 = _mm_mul_ps(yyyy, rhs.row[1].ps);
				__m128 e2 = _mm_mul_ps(zzzz, rhs.row[2].ps);
				__m128 e3 = _mm_mul_ps(wwww, rhs.row[3].ps);

				__m128 e01 = _mm_add_ps(e0, e1);
				__m128 e02 = _mm_add_ps(e2, e3);

				row[i].ps = _mm_add_ps(e01, e02);
			}

			return *this;
		}

		inline void* operator new(size_t size) { void* p = _aligned_malloc(size, 16); return p; }
		inline void operator delete(void* p) { _aligned_free(p); }
		//--------------------------------------------------------
		
		// Friend Operator Overloadings --------------------------
		__forceinline friend const Matrix44 operator*(const float lhs, const Matrix44& rhs)
		{
			Matrix44 result {
				lhs * rhs.row[0],
				lhs * rhs.row[1], 
				lhs * rhs.row[2], 
				lhs * rhs.row[3] 
			};
			
			return result;
		}

		__forceinline friend const bool operator==(const Matrix44& lhs, const Matrix44& rhs)
		{
			return (lhs.row[0] == rhs.row[0] && 
					lhs.row[1] == rhs.row[1] && 
					lhs.row[2] == rhs.row[2] && 
					lhs.row[3] == rhs.row[3]);
		}

		__forceinline friend const bool operator!=(const Matrix44& lhs, const Matrix44& rhs)
		{
			return !(lhs == rhs);
		}

		friend std::ostream& operator<<(std::ostream& os, const Matrix44& rhs)
		{
			for (int i = 0; i < 4; i++)
			{
				os << "| ";
				for (int j = 0; j < 4; j++)
				{
					os << rhs.elem[4 * i + j] << " ";
				}
				os << " |\n";
			}
			return os;
		}
		//--------------------------------------------------------
	};

	using Mat44f = Matrix44;
}