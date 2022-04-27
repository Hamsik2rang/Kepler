#pragma once

#include "gtest/gtest.h"

#include "Math/Vector.h"


TEST(Vector2D_Func_GetData_Test, Vector2DTest)
{
	using namespace kepler::math;

	Vector2D<float> v{ 1.0f, 3.0f };
	float* pv = v.GetData();

	EXPECT_EQ(&v.x, pv);
	EXPECT_EQ(*pv + *(pv + 1), v.x + v.y);
}

TEST(Vector2D_Operation_Assign_float_Test, Vector2DTest)
{
	using namespace kepler::math;
	/////////////// Vector<float>////////////////////////
	{
		Vec2f a;
		Vec2f b{ 3.0f, 2.0f };
		a = b;
		EXPECT_EQ(a, b);

		Vec2f c = a;
		c = c;
		EXPECT_EQ(a, c);
	}
}

TEST(Vector2D_Operation_Assign_int_Test, Vector2DTest)
{
	using namespace kepler::math;
	Vec2i a;
	Vec2i b{ 3, 2 };
	a = b;
	EXPECT_EQ(a, b);

	Vec2i c = a;
	c = c;
	EXPECT_EQ(a, c);
}

TEST(Vecor2D_Operation_Assign_char_Test, Vector2DTest)
{
	using namespace kepler::math;

	Vec2c a;
	Vec2c b{ (char)3, (char)2 };
	a = b;
	EXPECT_EQ(a, b);

	Vec2c c = a;
	c = c;
	EXPECT_EQ(a, c);
}

TEST(Vector2D_Operation_Assign_uchar_Test, Vector2DTest)
{
	using namespace kepler::math;

	Vec2b a;
	Vec2b b{ 3, 2 };
	a = b;
	EXPECT_EQ(a, b);

	Vec2b c = a;
	c = c;
	EXPECT_EQ(a, c);
}

TEST(Vector2D_Operation_Plus_float_Test, Vector2DTest)
{
	using namespace kepler::math;

	Vec2f a{ 1.0f ,3.0f };
	Vec2f b{ 3.0f, 1.0f };

	// 교환법칙
	EXPECT_EQ(a + b, b + a);

	// 결합법칙
	Vec2f c{ 4.0f ,-1.0f };
	EXPECT_EQ((a + b) + c, a + (b + c));

	// 항등원
	EXPECT_EQ(c + Vec2f::Zero, c);
}

TEST(Vector2D_Operation_Puls_int_Test, Vector2DTest)
{
	using namespace kepler::math;
	Vec2i a{ 1, 3 };
	Vec2i b{ 3, 1 };

	// 교환법칙
	EXPECT_EQ(a + b, b + a);

	// 결합법칙
	Vec2i c{ 4, 1 };
	EXPECT_EQ((a + b) + c, a + (b + c));

	// 항등원
	EXPECT_EQ(c + Vec2i::Zero, c);
}
TEST(Vector2D_Operation_Plus_char_Test, Vector2DTest)
{
	using namespace kepler::math;
	Vec2c a{ 1, 3 };
	Vec2c b{ 3, 1 };

	// 교환법칙
	EXPECT_EQ(a + b, b + a);

	// 결합법칙
	Vec2c c{ 4, 1 };
	EXPECT_EQ((a + b) + c, a + (b + c));

	// 항등원
	EXPECT_EQ(c + Vec2c::Zero, c);
}
TEST(Vector2D_Operation_Plus_uchar_Test, Vector2DTest)
{
	using namespace kepler::math;
	Vec2b a{ 1, 3 };
	Vec2b b{ 3, 1 };

	// 교환법칙
	EXPECT_EQ(a + b, b + a);

	// 결합법칙
	Vec2b c{ 4, 1 };

	EXPECT_EQ((a + b) + c, a + (b + c));
	// 항등원
	EXPECT_EQ(c + Vec2b::Zero, c);

}

TEST(Vector2D_Operation_Minus_float_Test, Vector2DTest)
{
	using namespace kepler::math;


	Vec2f a(1.0f, 3.0f);
	Vec2f b(1.0f, 1.0f);

	// 교환법칙 - 성립하면 안됨
	Vec2f t1 = a - b;
	Vec2f t2 = b - a;
	EXPECT_NE(t1, t2);

	// 결합법칙
	Vec2f c(4.0f, 1.0f);
	EXPECT_NE((a - b) - c, a - (b - c));

	Vec2f d(4.0f, 1.0f);
	EXPECT_EQ(c - d, Vec2f::Zero);

	Vec2f e;
	e = d;
	e - Vec2f::Zero;
	EXPECT_EQ(d, e);
}
TEST(Vector2D_Operation_Minus_int_Test, Vector2DTest)
{
	using namespace kepler::math;

	Vec2i a(3, 1);
	Vec2i b(1, 1);

	// 교환법칙 - 성립하면 안됨
	EXPECT_NE(a - b, b - a);

	// 결합법칙
	Vec2i c(4, 1);
	EXPECT_NE((a - b) - c, a - (b - c));

	Vec2i d(4, 1);
	EXPECT_EQ(c - d, Vec2i::Zero);

	Vec2i e;
	e = d;
	e - Vec2i::Zero;
	EXPECT_EQ(d, e);
}
TEST(Vector2D_Operation_Minus_char_Test, Vector2DTest)
{
	using namespace kepler::math;

	Vec2c a(1, 3);
	Vec2c b(1, 1);

	// 교환법칙 - 성립하면 안됨
	EXPECT_NE(a - b, b - a);

	// 결합법칙
	Vec2c c(4, 1);
	EXPECT_NE((a - b) - c, a - (b - c));

	Vec2c d(4, 1);
	EXPECT_EQ(c - d, Vec2c::Zero);

	Vec2c e;
	e = d;
	e - Vec2c::Zero;
	EXPECT_EQ(d, e);
}

TEST(Vector2D_Operation_Minus_uchar_Test, Vector2DTest)
{
	using namespace kepler::math;

	Vec2b a(1, 3);
	Vec2b b(1, 1);

	// 교환법칙 - 성립하면 안됨
	EXPECT_NE(a - b, b - a);

	// 결합법칙
	Vec2b c(4, 1);
	EXPECT_NE((a - b) - c, a - (b - c));

	Vec2b d(4, 1);
	EXPECT_EQ(c - d, Vec2b::Zero);

	Vec2b e;
	e = d;
	e - Vec2b::Zero;
	EXPECT_EQ(d, e);
}

TEST(Vector2D_Length_Test, Vector2DTest)
{
	using namespace kepler::math;
	Vec2f a(3.0f, 4.0f);

	EXPECT_EQ(a.Length(), 5.0f);
	Vec2f b = -a;
	EXPECT_EQ(a.Length(), b.Length());
}

TEST(Vector2D_Dot_Product_Test, Vector2DTest)
{
	using namespace kepler::math;
	Vec2f a(1.0f, 0.0f);
	Vec2f b(0.0f, 1.0f);

	EXPECT_EQ(Dot(a, b), 0.0f);
	EXPECT_EQ(Dot(a, a), 1.0f);

}

TEST(Vector2D_Affine_Transformation_float_Test, Vector2DTest)
{
	using namespace kepler::math;
	Vec2f a(3.0f, 4.0f);
	auto b = a.ToAffine();
	Vec3f c(3.0f, 4.0f, 1.0f);
	EXPECT_EQ(b, c);
}

TEST(Vector2D_Affine_Transformation_int_Test, Vector2DTest)
{
	using namespace kepler::math;
	Vec2i a(3, 4);
	auto b = a.ToAffine();
	Vec3i c(3, 4, 1);
	EXPECT_EQ(b, c);
}

TEST(Vector2D_Affine_Transformation_char_Test, Vector2DTest)
{
	using namespace kepler::math;
	Vec2c a(3, 4);
	auto b = a.ToAffine();
	Vec3c c(3, 4, 1);
	EXPECT_EQ(b, c);
}

TEST(Vector2D_Affine_Transformation_uchar_Test, Vector2DTest)
{
	using namespace kepler::math;
	Vec2b a(3, 4);
	auto b = a.ToAffine();
	Vec3b c(3, 4, 1);
	EXPECT_EQ(b, c);
}

TEST(Vector2D_Operator_Indexer_Test, Vector2DTest)
{
	using namespace kepler::math;
	Vec2f a(1.0f, 3.0f);

	EXPECT_EQ(a[0], a.x);
	EXPECT_EQ(a[1], a.y);
}