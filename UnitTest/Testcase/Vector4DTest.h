#pragma once

#include "gtest/gtest.h"

#include "Math/Vector.h"

TEST(Vector4D_Unary_Operator_float_Test, Vector4DTest)
{
	using namespace kepler::math;
	Vec4f a = Vec4f::Up;
	EXPECT_EQ(-a, Vec4f::Down);
}

TEST(Vector4D_Unary_Operator_int_Test, Vector4DTest)
{
	using namespace kepler::math;
	Vec4i a = Vec4i::Up;
	EXPECT_EQ(-a, Vec4i::Down);

	Vec4i b(0, 1, -1, 0);
	EXPECT_EQ(-b.b, b.g);
}

TEST(Vector4D_Unary_Operator_char_Test, Vector4DTest)
{
	using namespace kepler::math;
	Vec4c a = Vec4c::Up;
	EXPECT_EQ(-a, Vec4c::Down);

	Vec4c b(0, 1, -1, 0);
	EXPECT_EQ(-b.b, b.g);
}

TEST(Vector4D_Unary_Operator_uchar_Test, Vector4DTest)
{
	using namespace kepler::math;
	Vec4b a = Vec4b::Up;
	EXPECT_EQ(-a, Vec4b::Down);

	Vec4b b(0, 1, -1, 0);
	EXPECT_EQ((unsigned char)-b.g, b.b);
}

TEST(Vector4D_Dot_Test, Vector4DTest)
{
	using namespace kepler::math;
	Vec4f a = Vec4f::Up;
	Vec4f b = Vec4f::Right;
	EXPECT_EQ(Dot(a, b), 0.0f);

	Vec4f c = Vec4f::Down;

	EXPECT_EQ(Dot(a, c), -1.0f);
	EXPECT_EQ(Dot(a, a), 1.0f);
}

TEST(Vector4D_Operator_Indexer_Test, Vector4DTest)
{
	using namespace kepler::math;
	Vec4f a(1.0f, 3.0f, 5.0f, 2.0f);

	EXPECT_EQ(a[0], a.x);
	EXPECT_EQ(a[1], a.y);
	EXPECT_EQ(a[2], a.z);
	EXPECT_EQ(a[3], a.w);
}

TEST(Vector4D_Operator_Plus_float_Test, Vector4DTest)
{
	using namespace kepler::math;
	Vec4f a(3.0f, 2.0f, 1.0f, 0.0f);
	Vec4f b(-3.0f, -2.0f, -1.0f, 0.0f);

	EXPECT_EQ(a + b, Vec4f::Zero);
	EXPECT_EQ(b + a, Vec4f::Zero);

	Vec4f c(10028.0f, 334924.0f, 532525.0f, 8813.0f);
	EXPECT_EQ(a + c, c + a);
}

TEST(Vector4D_Operator_Plus_int_Test, Vector4DTest)
{
	using namespace kepler::math;
	Vec4i a(3, 2, 1, 0);
	Vec4i b(-3, -2, -1, 0);
	EXPECT_EQ(a + b, Vec4i::Zero);
	EXPECT_EQ(b + a, Vec4i::Zero);
	Vec4i c(10028, 334924, 532525, 8813);
	EXPECT_EQ(a + c, c + a);
}

TEST(Vector4D_Operator_Plus_char_Test, Vector4DTest)
{
	using namespace kepler::math;
	Vec4c a(3, 2, 1, 0);
	Vec4c b(-3, -2, -1, 0);
	EXPECT_EQ(a + b, Vec4c::Zero);
	EXPECT_EQ(b + a, Vec4c::Zero);
	Vec4c c(10028, 334924, 532525, 8813);
	EXPECT_EQ(a + c, c + a);
}

TEST(Vector4D_Operator_Minus_float_Test, Vector4DTest)
{
	using namespace kepler::math;

	Vec4f a(3.0f, 2.0f, 1.0f, 0.0f);
	
	EXPECT_EQ(a - a, Vec4f::Zero);
	Vec4f b(1.0f, 3.0f, 99.0f, -138.0f);
	EXPECT_NE(a - b, b - a);
}

TEST(Vector4D_Operator_Multiply_flaot_Test, Vector4DTest)
{
	using namespace kepler::math;

	Vec4f a(3.0f, 2.0f, 1.0f, 0.0f);
	a = a * 3.2f;

	EXPECT_EQ(a, Vec4f(9.6f, 6.4f, 3.2f, 0.0f));
}

TEST(Vector4D_Normalize_Test, Vector4DTest)
{
	using namespace kepler::math;
	Vec4f a(33.2424f, 10012313.0f, 324.0f, 887.7f);
	EXPECT_EQ(a.Normalize().Length(), 1.0f);
}

