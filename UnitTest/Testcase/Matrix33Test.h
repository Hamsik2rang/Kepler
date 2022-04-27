#pragma once

#include "gtest/gtest.h"

#include "Math/Matrix33.hpp"

// Test cases
// pre. 비교연산(없으면 테스트 자체가 불가)
// 1. 단항연산
// 2. static 멤버 변수
// 3. Transpose
// 4. 사칙연산(+, -, *, /(only scalar))

TEST(Unary_Operator_Test, Matrix33Test)
{
	using namespace kepler::math;

	Mat33f m{
		1.0f,	3.0f,		1.0f,
		3.0f,	2.0f,		5.0f,
		888.4f,	123141.0f, -3324241.0f 
	};

	Mat33f n{
		-1.0f,		-3.0f,		-1.0f,
		-3.0f,		-2.0f,		-5.0f,
		-888.4f,	-123141.0f, 3324241.0f
	};

	EXPECT_EQ(-m, n);

	Mat33f o = Mat33f::Zero;
	EXPECT_EQ(o, -o);
}

TEST(Indexer_Test, Matrix3Test)
{
	using namespace kepler::math;

	Mat33f a{
		-1.0f,		-3.0f,		-4.0f,
		63.0f,		-2.0f,		-5.0f,
		-888.4f,	123141.0f, 3324241.0f
	};

	EXPECT_EQ(a[0][0], -1.0f);
	EXPECT_EQ(a[0][1], -3.0f);
	EXPECT_EQ(a[0][2], -4.0f);
	EXPECT_EQ(a[1][0], 63.0f);
	EXPECT_EQ(a[1][1], -2.0f);
	EXPECT_EQ(a[1][2], -5.0f);
	EXPECT_EQ(a[2][0], -888.4f);
	EXPECT_EQ(a[2][1], 123141.0f);
	EXPECT_EQ(a[2][2], 3324241.0f);
}

TEST(Static_Variable_Test, Matrix33Test)
{
	using namespace kepler::math;

	Mat33f m;

	Mat33f zero = {
		0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	
	Mat33f identity = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f 
	};

	EXPECT_EQ(m, Mat33f::Zero);
	EXPECT_EQ(zero, Mat33f::Zero);
	EXPECT_EQ(identity, Mat33f::Identity);

	m = {
		3.0f, 2.0f, 8.0f,
		99124.0f, 8834.4f, -1402.834f,
		4347.994f, 874.1f, -3545.0f
	};

	EXPECT_EQ(m * Mat33f::Identity, m);
}

TEST(Transpose_Test, Matrix33Test)
{
	using namespace kepler::math;

	Mat33f m(
		1.0f, 2.0f, 3.0f,
		1.0f, 2.0f, 3.0f,
		1.0f, 2.0f, 3.0f
	);

	Mat33f n(
		1.0f, 1.0f, 1.0f,
		2.0f, 2.0f, 2.0f,
		3.0f, 3.0f, 3.0f
	);

	EXPECT_EQ(m.Transpose(), n);
	EXPECT_EQ(Mat33f::Identity.Transpose(), Mat33f::Identity);
}

TEST(Plus_Operator_Test, Matrix33Test)
{
	using namespace kepler::math;

	Mat33f a(
		1.0f, 1.0f, 1.0f,
		3.0f, 4.0f, 5.0f,
		-88.34f, -1242987.0f, 3242.995f
	);

	Mat33f b(
		77645.3f, 224.0f, 965.13f,
		221.998f, 883.9f, 454.928f,
		0.0f, 823.2f, 11.18f
	);

	Mat33f c(
		a[0][0] + b[0][0], a[0][1] + b[0][1], a[0][2] + b[0][2],
		a[1][0] + b[1][0], a[1][1] + b[1][1], a[1][2] + b[1][2],
		a[2][0] + b[2][0], a[2][1] + b[2][1], a[2][2] + b[2][2]
	);

	EXPECT_EQ(a + b, c);
	EXPECT_EQ(a + Mat33f::Zero, a);
	EXPECT_EQ(a + b, b + a);
	EXPECT_EQ((a + b) + c, a + (b + c));
}

TEST(Minus_Operator_Test, Matrix33Test)
{
	using namespace kepler::math;

	Mat33f a(3.8f, 2.4f, 1.2f,
		88.02f, 77.78f, 55.94f,
		247.4f, 98.765f, 1124.224f
	);

	Mat33f b(0.0f, 3.0f, 8.4f,
		13.24f, 44.96f, 54.51f,
		876.543f, 250.965f, 974.345f
	);

	Mat33f c(
		a[0][0] - b[0][0], a[0][1] - b[0][1], a[0][2] - b[0][2],
		a[1][0] - b[1][0], a[1][1] - b[1][1], a[1][2] - b[1][2],
		a[2][0] - b[2][0], a[2][1] - b[2][1], a[2][2] - b[2][2]
		);

	EXPECT_EQ(a - b, c);
	EXPECT_EQ(a - Mat33f::Zero, a);
	EXPECT_NE(a - b, b - a);
	EXPECT_NE((a - b) - c, a - (b - c));
}

TEST(Multiply_Operator_Test, Matrix33Test)
{
	using namespace kepler::math;

	// with matrix
	Mat33f a(
		1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f,
		7.0f, 8.0f, 9.0f
	);

	EXPECT_EQ(a * Mat33f::Identity, a);
	EXPECT_EQ(a * Mat33f::Zero, Mat33f::Zero);

	Mat33f b(
		1.0f, 3.0f, 5.0f,
		88.34f, 42.45f, 25.26f,
		124.428f, 339.985f, 876.541f
	);
	Mat33f c = Mat33f::Zero;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	EXPECT_EQ(a * b, c);

	// with scalar
	Mat33f m(
		1.0f, 2.0f, 3.0f, 
		4.0f, 5.0f, 6.0f, 
		7.0f, 8.0f, 9.0f
	);

	float f = 3.3f;
	Mat33f n(
		f * 1.0f, f * 2.0f, f * 3.0f,
		f * 4.0f, f * 5.0f, f * 6.0f,
		f * 7.0f, f * 8.0f, f * 9.0f
	);

	EXPECT_EQ(m * f, f * m);
	EXPECT_EQ(m * f, n);
}

TEST(Divide_Operator_Test, Matrix33Test)
{
	using namespace kepler::math;

	Mat33f m(
		1.0f, 3.0f, 5.0f,
		88.34f, 42.45f, 25.26f,
		124.428f, 339.985f, 876.541f
	);

	float f = -8.4f;

	Mat33f n(
		1.0f / f,		3.0f / f,		5.0f / f,
		88.34f / f,		42.45f / f,		25.26f / f,
		124.428f / f,	339.985f / f,	876.541f / f
	);
	EXPECT_EQ(m / f, n);
}