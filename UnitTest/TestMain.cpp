#include "kepch.h"
#include "pch.h"

#include "InputManagerTest.h"

// Example: 
//TEST(TestCaseName, TestName)
//{
//	EXPECT_EQ(1, 1);		// parameter 1, 2�� ������ ���
//	EXPECT_TRUE(true);		// parameter�� ������ ������ true�� ��ȯ�ϸ� ���
//  EXPECT_FALSE(false);	// parameter�� ������ ������ false�� ��ȯ�ϸ� ���
//  EXPECT_THROW(...);		// parameter�� ������ ������ ���ܸ� throw�ϸ� ���(���� ����)
//}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
