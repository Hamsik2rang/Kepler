#pragma once

#include <iostream>
#include <stdint.h>

#include "Base.h"
#include "Intrinsic.h"
#include "Vector4.hpp"


namespace kepler {
// row-majer matrix
struct KEPLER_MATH_API alignas(16) Matrix44
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
		__m128 z0123 = _mm_shuffle_ps(z0011, z2233, KP_MM_SHUFFLE_LE(0, 2, 0, 2));

		_mm_storeu_ps(result.row[2].elem, z0123);

		// make w0 w1 w2 w3
		__m128 w0011 = _mm_shuffle_ps(row0ps, row1ps, KP_MM_SHUFFLE_LE(3, 3, 3, 3));
		__m128 w2233 = _mm_shuffle_ps(row2ps, row3ps, KP_MM_SHUFFLE_LE(3, 3, 3, 3));
		__m128 w0123 = _mm_shuffle_ps(w0011, w2233, KP_MM_SHUFFLE_LE(0, 2, 0, 2));

		_mm_storeu_ps(result.row[3].elem, w0123);

		return result;
	};

	__forceinline const Matrix44 Inverse() const
	{
		__m128 vTemp1 = _mm_shuffle_ps(_mm_load_ps(&row[0].r), _mm_load_ps(&row[1].r), _MM_SHUFFLE(1, 0, 1, 0));
		__m128 vTemp2 = _mm_shuffle_ps(_mm_load_ps(&row[2].r), _mm_load_ps(&row[3].r), _MM_SHUFFLE(1, 0, 1, 0));
		__m128 vTemp3 = _mm_shuffle_ps(_mm_load_ps(&row[0].r), _mm_load_ps(&row[1].r), _MM_SHUFFLE(3, 2, 3, 2));
		__m128 vTemp4 = _mm_shuffle_ps(_mm_load_ps(&row[2].r), _mm_load_ps(&row[3].r), _MM_SHUFFLE(3, 2, 3, 2));

		Matrix44 mat;
		mat.row[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
		mat.row[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
		mat.row[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
		mat.row[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));

		__m128 mr0 = _mm_load_ps(&mat.row[0].r);
		__m128 mr1 = _mm_load_ps(&mat.row[1].r);
		__m128 mr2 = _mm_load_ps(&mat.row[2].r);
		__m128 mr3 = _mm_load_ps(&mat.row[3].r);


		__m128 v00 = KP_PERMUTE_PS(mr2, _MM_SHUFFLE(1, 1, 0, 0));
		__m128 v10 = KP_PERMUTE_PS(mr3, _MM_SHUFFLE(3, 2, 3, 2));
		__m128 v01 = KP_PERMUTE_PS(mr0, _MM_SHUFFLE(1, 1, 0, 0));
		__m128 v11 = KP_PERMUTE_PS(mr1, _MM_SHUFFLE(3, 2, 3, 2));
		__m128 v02 = _mm_shuffle_ps(mr2, mr0, _MM_SHUFFLE(2, 0, 2, 0));
		__m128 v12 = _mm_shuffle_ps(mr3, mr1, _MM_SHUFFLE(3, 1, 3, 1));

		__m128 d0 = _mm_mul_ps(v00, v10);
		__m128 d1 = _mm_mul_ps(v01, v11);
		__m128 d2 = _mm_mul_ps(v02, v12);

		v00 = KP_PERMUTE_PS(mr2, _MM_SHUFFLE(3, 2, 3, 2));
		v10 = KP_PERMUTE_PS(mr3, _MM_SHUFFLE(1, 1, 0, 0));
		v01 = KP_PERMUTE_PS(mr0, _MM_SHUFFLE(3, 2, 3, 2));
		v11 = KP_PERMUTE_PS(mr1, _MM_SHUFFLE(1, 1, 0, 0));
		v02 = _mm_shuffle_ps(mr2, mr0, _MM_SHUFFLE(3, 1, 3, 1));
		v12 = _mm_shuffle_ps(mr3, mr1, _MM_SHUFFLE(2, 0, 2, 0));

		d0 = KP_FNMADD_PS(v00, v10, d0);
		d1 = KP_FNMADD_PS(v01, v11, d1);
		d2 = KP_FNMADD_PS(v02, v12, d2);

		v11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 1, 3, 1));
		v00 = KP_PERMUTE_PS(mr1, _MM_SHUFFLE(1, 0, 2, 1));
		v10 = _mm_shuffle_ps(v11, d0, _MM_SHUFFLE(0, 3, 0, 2));
		v01 = KP_PERMUTE_PS(mr0, _MM_SHUFFLE(0, 1, 0, 2));
		v11 = _mm_shuffle_ps(v11, d0, _MM_SHUFFLE(2, 1, 2, 1));

		__m128 v13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 3, 3, 1));
		v02 = KP_PERMUTE_PS(mr3, _MM_SHUFFLE(1, 0, 2, 1));
		v12 = _mm_shuffle_ps(v13, d1, _MM_SHUFFLE(0, 3, 0, 2));
		__m128 v03 = KP_PERMUTE_PS(mr2, _MM_SHUFFLE(0, 1, 0, 2));
		v13 = _mm_shuffle_ps(v13, d1, _MM_SHUFFLE(2, 1, 2, 1));

		__m128 c0 = _mm_mul_ps(v00, v10);
		__m128 c2 = _mm_mul_ps(v01, v11);
		__m128 c4 = _mm_mul_ps(v02, v12);
		__m128 c6 = _mm_mul_ps(v03, v13);

		v11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(0, 0, 1, 0));
		v00 = KP_PERMUTE_PS(mr1, _MM_SHUFFLE(2, 1, 3, 2));
		v10 = _mm_shuffle_ps(d0, v11, _MM_SHUFFLE(2, 1, 0, 3));
		v01 = KP_PERMUTE_PS(mr0, _MM_SHUFFLE(1, 3, 2, 3));
		v11 = _mm_shuffle_ps(d0, v11, _MM_SHUFFLE(0, 2, 1, 2));

		v13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(2, 2, 1, 0));
		v02 = KP_PERMUTE_PS(mr3, _MM_SHUFFLE(2, 1, 3, 2));
		v12 = _mm_shuffle_ps(d1, v13, _MM_SHUFFLE(2, 1, 0, 3));
		v03 = KP_PERMUTE_PS(mr2, _MM_SHUFFLE(1, 3, 2, 3));
		v13 = _mm_shuffle_ps(d1, v13, _MM_SHUFFLE(0, 2, 1, 2));

		c0 = KP_FNMADD_PS(v00, v10, c0);
		c2 = KP_FNMADD_PS(v01, v11, c2);
		c4 = KP_FNMADD_PS(v02, v12, c4);
		c6 = KP_FNMADD_PS(v03, v13, c6);

		v00 = KP_PERMUTE_PS(mr1, _MM_SHUFFLE(0, 3, 0, 3));
		v10 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 0, 2, 2));
		v10 = KP_PERMUTE_PS(v10, _MM_SHUFFLE(0, 2, 3, 0));
		v01 = KP_PERMUTE_PS(mr0, _MM_SHUFFLE(2, 0, 3, 1));
		v11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 0, 3, 0));
		v11 = KP_PERMUTE_PS(v11, _MM_SHUFFLE(2, 1, 0, 3));
		v02 = KP_PERMUTE_PS(mr3, _MM_SHUFFLE(0, 3, 0, 3));
		v12 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 2, 2, 2));
		v12 = KP_PERMUTE_PS(v12, _MM_SHUFFLE(0, 2, 3, 0));
		v03 = KP_PERMUTE_PS(mr2, _MM_SHUFFLE(2, 0, 3, 1));
		v13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 2, 3, 0));
		v13 = KP_PERMUTE_PS(v13, _MM_SHUFFLE(2, 1, 0, 3));

		v00 = _mm_mul_ps(v00, v10);
		v01 = _mm_mul_ps(v01, v11);
		v02 = _mm_mul_ps(v02, v12);
		v03 = _mm_mul_ps(v03, v13);
		
		__m128 c1 = _mm_sub_ps(c0, v00);
		c0 = _mm_add_ps(c0, v00);
		__m128 c3 = _mm_add_ps(c2, v01);
		c2 = _mm_sub_ps(c2, v01);
		__m128 c5 = _mm_sub_ps(c4, v02);
		c4 = _mm_add_ps(c4, v02);
		__m128 c7 = _mm_add_ps(c6, v03);
		c6 = _mm_sub_ps(c6, v03);

		c0 = _mm_shuffle_ps(c0, c1, _MM_SHUFFLE(3, 1, 2, 0));
		c2 = _mm_shuffle_ps(c2, c3, _MM_SHUFFLE(3, 1, 2, 0));
		c4 = _mm_shuffle_ps(c4, c5, _MM_SHUFFLE(3, 1, 2, 0));
		c6 = _mm_shuffle_ps(c6, c7, _MM_SHUFFLE(3, 1, 2, 0));

		c0 = KP_PERMUTE_PS(c0, _MM_SHUFFLE(3, 1, 2, 0));
		c2 = KP_PERMUTE_PS(c2, _MM_SHUFFLE(3, 1, 2, 0));
		c4 = KP_PERMUTE_PS(c4, _MM_SHUFFLE(3, 1, 2, 0));
		c6 = KP_PERMUTE_PS(c6, _MM_SHUFFLE(3, 1, 2, 0));

		// Get the Determinant
		float c0dotmr0 = Dot(Vec4f(c0), Vec4f(mr0));
		__m128 vTemp = _mm_set1_ps(c0dotmr0);
		vTemp = _mm_div_ps(_mm_set1_ps(1.0f), vTemp);
		Matrix44 result;
		result.row[0] = _mm_mul_ps(c0, vTemp);
		result.row[1] = _mm_mul_ps(c2, vTemp);
		result.row[2] = _mm_mul_ps(c4, vTemp);
		result.row[3] = _mm_mul_ps(c6, vTemp);

		return result;
	}
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
		Matrix44 result{
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