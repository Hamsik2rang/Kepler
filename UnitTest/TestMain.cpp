#include "kepch.h"
#include "pch.h"

#include "InputManagerTest.h"

// Example: 
//TEST(TestCaseName, TestName)
//{
//	EXPECT_EQ(1, 1);		// parameter 1, 2가 같으면 통과
//	EXPECT_TRUE(true);		// parameter로 전달한 동작이 true를 반환하면 통과
//  EXPECT_FALSE(false);	// parameter로 전달한 동작이 false를 반환하면 통과
//  EXPECT_THROW(...);		// parameter로 전달한 동작이 예외를 throw하면 통과(종류 무관)
//}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
