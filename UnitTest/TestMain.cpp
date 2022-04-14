#include "kepch.h"
#include "pch.h"

#include "InputManagerTest.h"

//
//TEST(TestCaseName, TestName)
//{
//	EXPECT_EQ(1, 1);
//	EXPECT_TRUE(true);
//}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
