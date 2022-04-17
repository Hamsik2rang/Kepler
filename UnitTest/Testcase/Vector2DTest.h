#pragma once

#include "Math/Vector2D.hpp"

TEST(Vector2D_Func_GetData_Test, Vector2DTest)
{
	using namespace kepler::math;

	Vector2D<float> v{ 1.0f, 3.0f };
	float* pv = v.GetData();

	EXPECT_EQ(&v.x, pv);
	EXPECT_EQ(*pv + *(pv + 1), v.x + v.y);
}

TEST(Vecor2D_Operation_Assign_Test, Vector2DTest)
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
	/////////////////////////////////////////////////////

	/////////////// Vector<int>////////////////////////
	{
		Vec2i a;
		Vec2i b{ 3, 2 };
		a = b;
		EXPECT_EQ(a, b);

		Vec2i c = a;
		c = c;
		EXPECT_EQ(a, c);
	}
	/////////////////////////////////////////////////////

	/////////////// Vector<char>////////////////////////
	{
		Vec2c a;
		Vec2c b{ (char)3, (char)2 };
		a = b;
		EXPECT_EQ(a, b);

		Vec2c c = a;
		c = c;
		EXPECT_EQ(a, c);
	}
	/////////////////////////////////////////////////////

	/////////////// Vector<byte>////////////////////////
	{
		Vec2b a;
		Vec2b b{ (unsigned char)3, (unsigned char)2 };
		a = b;
		EXPECT_EQ(a, b);

		Vec2b c = a;
		c = c;
		EXPECT_EQ(a, c);
	}
	/////////////////////////////////////////////////////

	//////////////////// EXCEPTED ///////////////////////
	////////// Type conversion(float->int)///////////////
	//{
	//	Vec2f f{ 3.0f, 2.0f };
	//	Vec2i a{ -1, -1 };
	//	//a = f;
	//	Vec2f ans = { 3, 2 };

	//	EXPECT_EQ(ans, a);
	//}
	/////////////////////////////////////////////////////

}

//TEST(Vector2D_Operation_Plus_Test, Vector2DTest)
//{
//	using namespace kepler::math;
//
//	/////////////// Vector<float>////////////////////////
//	{
//		Vec2f a{ 1.0f ,3.0f };
//		Vec2f b{ 3.0f, 1.0f };
//
//		// 교환법칙
//		EXPECT_EQ(a + b, b + a);
//
//		// 결합법칙
//		Vec2f c{ 4.0f ,-1.0f };
//		EXPECT_EQ((a + b) + c, a + (b + c));
//	}
//	/////////////////////////////////////////////////////
//	
//	/////////////// Vector<int>//////////////////////////
//	{
//		Vec2i a{ 1, 3 };
//		Vec2i b{ 3, 1 };
//
//		// 교환법칙
//		EXPECT_EQ(a + b, b + a);
//
//		// 결합법칙
//		Vec2i c{ 4, 1 };
//		EXPECT_EQ((a + b) + c, a + (b + c));
//	}
//	/////////////////////////////////////////////////////
//
//	/////////////// Vector<char>//////////////////////////
//	{
//		Vec2c a{ (char)1, (char)3 };
//		Vec2c b{ (char)3, (char)1 };
//
//		// 교환법칙
//		EXPECT_EQ(a + b, b + a);
//
//		// 결합법칙
//		Vec2c c{ 4, 1 };
//		EXPECT_EQ((a + b) + c, a + (b + c));
//	}
//	/////////////////////////////////////////////////////
//	
//	/////////////// Vector<bit>//////////////////////////
//	{
//		Vec2b a{ (unsigned char)1, (unsigned char)3 };
//		Vec2b b{ (unsigned char)3, (unsigned char)1 };
//
//		// 교환법칙
//		EXPECT_EQ(a + b, b + a);
//
//		// 결합법칙
//		Vec2b c{ 4, 1 };
//		EXPECT_EQ((a + b) + c, a + (b + c));
//	}
//	/////////////////////////////////////////////////////
//}

//TEST(Vector2D_Operation_Minus_Test, Vector2DTest)
//{
//	using namespace kepler::math;
//
//	/////////////// Vector<float>//////////////////////////
//	{
//		Vec2f a{ 1.0f, 3.0f };
//		Vec2f b{ 1.0f, 1.0f };
//
//		// 교환법칙 - 성립하면 안됨
//		EXPECT_NE(a - b, b - a);
//
//		// 결합법칙
//		Vec2f c{ 4.0f, 1.0f };
//		EXPECT_NE((a - b) - c, a - (b - c));
//
//		Vec2f d{ 4.0f, 1.0f };
//		EXPECT_EQ(c - d, Vec2f::Zero());
//
//		Vec2f e;
//		e = d;
//		e - Vec2f::Zero();
//		EXPECT_EQ(d, e);
//	}
//	/////////////////////////////////////////////////////
//}
//
