#pragma once
#include "pch.h"

#include <vector>
#include "Math/Matrix22.hpp"

using namespace kepler::math;

// Test cases
// pre. 비교연산(없으면 테스트 자체가 불가)
// 1. 단항연산
// 2. static 변수에 값 정확히 들어있는지
// 3. 사칙연산(+, -, *, /(only scalar))
// 4. Transpose

TEST(Unary_Operator_Test, Matrix22Test)
{
	Mat22f a = { 3.0f, 4.0f, 2.0f, 3.0f };
	Mat22f b = { {-3.0f, -4.0f}, {-2.0f, -3.0f} };

	EXPECT_EQ(-a, b);
	EXPECT_EQ(a, +a);
	EXPECT_EQ(b, +b);
}

TEST(Assign_Operator_Test, Matrix22Test)
{
	Mat22f a;
	Mat22f b = Mat22f::Identity;
	a = b;
	EXPECT_EQ(a, Mat22f::Identity);
}

TEST(Static_Variable_Test, Matrix22Test)
{
	Mat22f zero = { 0.0f,0.0f, 0.0f, 0.0f };
	Mat22f identity = { {1.0f, 0.0f}, {0.0f, 1.0f} };

	EXPECT_EQ(Mat22f::Zero, zero);
	EXPECT_EQ(Mat22f::Identity, identity);
}

TEST(Plus_Operator_Test, Matrix22Test)
{
	Mat22f a{ {3.0f, 2.0f}, {2.0f, 1.0f} };
	Mat22f b{ {4.0f, 3.0f}, {3.0f, 2.0f} };
	Mat22f c{ {7.0f, 5.0f}, {5.0f, 3.0f} };

	EXPECT_EQ(a + b, c);
	EXPECT_EQ(a + Mat22f::Zero, a);
	EXPECT_EQ(a + b, b + a);
	EXPECT_EQ((a + b) + c, a + (b + c));
}

TEST(Minus_Operator_Test, Matrix22Test)
{
	Mat22f a{ {3.0f, 2.0f}, {2.0f, 1.0f} };
	Mat22f b{ {4.0f, 3.0f}, {3.0f, 2.0f} };
	Mat22f c{ {-1.0f, -1.0f}, {-1.0f, -1.0f} };

	EXPECT_EQ(a - b, c);
	EXPECT_EQ(a - Mat22f::Zero, a);
	EXPECT_NE(a - b, b - a);
	EXPECT_NE((a - b) - c, a - (b - c));
}

TEST(Multiply_Operator_Test, Matrix22Test)
{
	// with matrix
	Mat22f a{ {1.0f, 0.0f}, {3.0f, 1.0f} };
	Mat22f b{ {3.0f, 2.0f}, {2.0f, 1.0f} };
	Mat22f c{ {3.0f, 2.0f}, {11.0f, 7.0f} };

	EXPECT_EQ(a * b, c);
	EXPECT_EQ(a * Mat22f::Identity, a);
	EXPECT_EQ(b * Mat22f::Identity, b);
	EXPECT_NE(a * b, b * a);

	// with scalar
	Mat22f x{ 3.0f, 2.0f, 5.0f, 4.0f };
	float f = -1.3f;
	Mat22f y{ 3.0f * (-1.3f), 2.0f * (-1.3f), 5.0f * (-1.3f), 4.0f * (-1.3f)};
	Mat22f z(-3.9f, -2.6f, -6.5f, -5.2f);
	EXPECT_EQ(x * f, y);
	EXPECT_EQ(f * x, y);

	EXPECT_EQ(x * f, f * x);

	// 부동소수점 오차
	//EXPECT_NEAR(x * f, z, 1.732e-4);
	//EXPECT_NEAR(f * x, z, 1.732e-4);
}

TEST(Divide_Operator_Test, Matrix22Test)
{
	Mat22f a{ {3.0f, 5.0f, 999.0f, -8824.4f} };
	float f = 3.0f;

	Mat22f b{ {1.0f, 5.0f / 3.0f, 333.0f, -8824.4f / 3.0f} };

	EXPECT_EQ(a / f, b);
}

TEST(Transpose_Test, Matrix22Test)
{
	Mat22f identity = Mat22f::Identity;

	EXPECT_EQ(identity.Transpose(), identity);
	
	Mat22f a{ 3.0f, 4.0f, 2.0f, 8.0f };
	Mat22f b{ 3.0f, 2.0f, 4.0f, 8.0f };

	EXPECT_EQ(a.Transpose(), b);
}