#include "pch.h"



TEST(TestCaseName, TestName)
{
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(ButtonNotDown, ButtonDownTest)
{
	EXPECT_FALSE(false);
}


	int main(int argc, char** argv)
	{
		::testing::InitGoogleTest(&argc, argv);
		return RUN_ALL_TESTS();
	}
