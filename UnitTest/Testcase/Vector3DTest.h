#pragma once

#include "gtest/gtest.h"

#include "Math/Vector.h"


TEST(Vector3D_Unary_Operator_float_Test, Vector3DTest)
{
	using namespace kepler::math;
	Vec3f a(-3.0f, -2.0f, 0.5f);

	Vec3f b(-a);
	EXPECT_TRUE(b == (a * -1));
	
	EXPECT_EQ(b.x, a.x * -1);
	EXPECT_EQ(b.y, a.y * -1);
	EXPECT_EQ(b.z, a.z * -1);
}

TEST(Vector3D_Unary_Operator_int_Test, Vector3DTest)
{
	using namespace kepler::math;
	Vec3i a(-3, -2, 0);

	Vec3i b(-a);
	EXPECT_TRUE(b == (a * -1));

	EXPECT_EQ(b.x, a.x * -1);
	EXPECT_EQ(b.y, a.y * -1);
	EXPECT_EQ(b.z, a.z * -1);
}

TEST(Vector3D_Unary_Operator_char_Test, Vector3DTest)
{
	using namespace kepler::math;
	Vec3c a(-3, -2, 0);

	Vec3c b(-a);
	EXPECT_TRUE(b == (a * -1));

	EXPECT_EQ(b.x, a.x * -1);
	EXPECT_EQ(b.y, a.y * -1);
	EXPECT_EQ(b.z, a.z * -1);
}

TEST(Vector3D_Assign_Operator_Test, Vector3DTest)
{
	using namespace kepler::math;
	Vec3f a(3.0f, 0.3f, 999999.8f);
	Vec3f b;
	b = a;

	EXPECT_EQ(a, b);
}

TEST(Vector3D_Dot_Test, Vector3DTest)
{
	using namespace kepler::math;
	EXPECT_EQ(Dot(Vec3f::Up, Vec3f::Right), 0.0f);
}

TEST(Vector3D_Operator_Indexer_Test, Vector3DTest)
{
	using namespace kepler::math;
	Vec3f a(1.0f, 3.0f, 5.0f);

	EXPECT_EQ(a[0], a.x);
	EXPECT_EQ(a[1], a.y);
	EXPECT_EQ(a[2], a.z);
}