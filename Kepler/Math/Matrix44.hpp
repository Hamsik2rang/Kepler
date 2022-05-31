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

			__m128 row0ps = _mm_loadu_ps(row[0].elem);
			__m128 row1ps = _mm_loadu_ps(row[1].elem);
			__m128 row2ps = _mm_loadu_ps(row[2].elem);
			__m128 row3ps = _mm_loadu_ps(row[3].elem);

			// make x0 x1 x2 x3
			__m128 x0011 = _mm_shuffle_ps(row0ps, row1ps, KP_MM_SHUFFLE_LE(0, 0, 0, 0));
			__m128 x2233 = _mm_shuffle_ps(row2ps, row3ps, KP_MM_SHUFFLE_LE(0, 0, 0, 0));
			__m128 x0123 = _mm_shuffle_ps(x0011, x2233, KP_MM_SHUFFLE_LE(0, 2, 0, 2));

			_mm_storeu_ps(result.row[0].elem, x0123);

			// make y0 y1 y2 y3
			__m128 y0011 = _mm_shuffle_ps(row0ps, row1ps, KP_MM_SHUFFLE_LE(1, 1, 1, 1));
			__m128 y2233 = _mm_shuffle_ps(row2ps, row3ps, KP_MM_SHUFFLE_LE(1, 1, 1, 1));
			__m128 y0123 = _mm_shuffle_ps(y0011, y2233, KP_MM_SHUFFLE_LE(0, 2, 0, 2));

			_mm_storeu_ps(result.row[1].elem, y0123);

			// make z0 z1 z2 z3 
			__m128 z0011 = _mm_shuffle_ps(row0ps, row1ps, KP_MM_SHUFFLE_LE(2, 2, 2, 2));
			__m128 z2233 = _mm_shuffle_ps(row2ps, row3ps, KP_MM_SHUFFLE_LE(2, 2, 2, 2));
			__m128 z0123 = _mm_shuffle_ps(z0011,z2233, KP_MM_SHUFFLE_LE(0, 2, 0, 2));

			_mm_storeu_ps(result.row[2].elem, z0123);

			// make w0 w1 w2 w3
			__m128 w0011 = _mm_shuffle_ps(row0ps, row1ps, KP_MM_SHUFFLE_LE(3, 3, 3, 3));
			__m128 w2233 = _mm_shuffle_ps(row2ps, row3ps, KP_MM_SHUFFLE_LE(3, 3, 3, 3));
			__m128 w0123 = _mm_shuffle_ps(w0011, w2233, KP_MM_SHUFFLE_LE(0, 2, 0, 2));

			_mm_storeu_ps(result.row[3].elem, w0123);

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
				__m128 rowips = _mm_loadu_ps(row[i].elem);

				__m128 xxxx = _mm_shuffle_ps(rowips, rowips, KP_MM_SHUFFLE_LE(0, 0, 0, 0));
				__m128 yyyy = _mm_shuffle_ps(rowips, rowips, KP_MM_SHUFFLE_LE(1, 1, 1, 1));
				__m128 zzzz = _mm_shuffle_ps(rowips, rowips, KP_MM_SHUFFLE_LE(2, 2, 2, 2));
				__m128 wwww = _mm_shuffle_ps(rowips, rowips, KP_MM_SHUFFLE_LE(3, 3, 3, 3));

				__m128 row0ps = _mm_loadu_ps(rhs.row[0].elem);
				__m128 row1ps = _mm_loadu_ps(rhs.row[1].elem);
				__m128 row2ps = _mm_loadu_ps(rhs.row[2].elem);
				__m128 row3ps = _mm_loadu_ps(rhs.row[3].elem);
				
				__m128 e0 = _mm_mul_ps(xxxx, row0ps);
				__m128 e1 = _mm_mul_ps(yyyy, row1ps);
				__m128 e2 = _mm_mul_ps(zzzz, row2ps);
				__m128 e3 = _mm_mul_ps(wwww, row3ps);

				__m128 e01 = _mm_add_ps(e0, e1);
				__m128 e02 = _mm_add_ps(e2, e3);

				_mm_storeu_ps(result.row[i].elem, _mm_add_ps(e01, e02));
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
				__m128 rowips = _mm_loadu_ps(row[i].elem);
				__m128 row0ps = _mm_loadu_ps(row[0].elem);
				__m128 row1ps = _mm_loadu_ps(row[1].elem);
				__m128 row2ps = _mm_loadu_ps(row[2].elem);
				__m128 row3ps = _mm_loadu_ps(row[3].elem);

				__m128 xxxx = _mm_shuffle_ps(row0ps, row0ps, KP_MM_SHUFFLE_LE(0, 0, 0, 0));
				__m128 yyyy = _mm_shuffle_ps(row1ps, row1ps, KP_MM_SHUFFLE_LE(1, 1, 1, 1));
				__m128 zzzz = _mm_shuffle_ps(row2ps, row2ps, KP_MM_SHUFFLE_LE(2, 2, 2, 2));
				__m128 wwww = _mm_shuffle_ps(row3ps, row3ps, KP_MM_SHUFFLE_LE(3, 3, 3, 3));

				__m128 rhs0ps = _mm_loadu_ps(rhs.row[0].elem);
				__m128 rhs1ps = _mm_loadu_ps(rhs.row[1].elem);
				__m128 rhs2ps = _mm_loadu_ps(rhs.row[2].elem);
				__m128 rhs3ps = _mm_loadu_ps(rhs.row[3].elem);

				__m128 e0 = _mm_mul_ps(xxxx, rhs0ps);
				__m128 e1 = _mm_mul_ps(yyyy, rhs1ps);
				__m128 e2 = _mm_mul_ps(zzzz, rhs2ps);
				__m128 e3 = _mm_mul_ps(wwww, rhs3ps);

				__m128 e01 = _mm_add_ps(e0, e1);
				__m128 e02 = _mm_add_ps(e2, e3);

				_mm_storeu_ps(row[i].elem, _mm_add_ps(e01, e02));
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

		__forceinline friend const Vector4 operator*(const Vector4& lhs, const Matrix44& rhs)
		{
			__m128 lps = _mm_loadu_ps(lhs.elem);

			__m128 xxxx = _mm_shuffle_ps(lps, lps, KP_MM_SHUFFLE_LE(0, 0, 0, 0));
			__m128 yyyy = _mm_shuffle_ps(lps, lps, KP_MM_SHUFFLE_LE(1, 1, 1, 1));
			__m128 zzzz = _mm_shuffle_ps(lps, lps, KP_MM_SHUFFLE_LE(2, 2, 2, 2));
			__m128 wwww = _mm_shuffle_ps(lps, lps, KP_MM_SHUFFLE_LE(3, 3, 3, 3));

			__m128 rhs0ps = _mm_loadu_ps(rhs.row[0].elem);
			__m128 rhs1ps = _mm_loadu_ps(rhs.row[1].elem);
			__m128 rhs2ps = _mm_loadu_ps(rhs.row[2].elem);
			__m128 rhs3ps = _mm_loadu_ps(rhs.row[3].elem);

			__m128 e0 = _mm_mul_ps(xxxx, rhs0ps);
			__m128 e1 = _mm_mul_ps(yyyy, rhs1ps);
			__m128 e2 = _mm_mul_ps(zzzz, rhs2ps);
			__m128 e3 = _mm_mul_ps(wwww, rhs3ps);

			__m128 e01 = _mm_add_ps(e0, e1);
			__m128 e02 = _mm_add_ps(e2, e3);

			Vector4 result(_mm_add_ps(e01, e02));

			return result;
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