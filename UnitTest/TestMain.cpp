#include "kepch.h"
#include "pch.h"

#include "Core/Input.h"

TEST(TestCaseName, TestName)
{
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(KeplerButtonNotDown, KeplerInputTest)
{
	EXPECT_FALSE(kepler::Input::IsButtonDown(1));
}

TEST(KeplerButtonDown, KeplerInputTest)
{
	EXPECT_TRUE(kepler::Input::IsButtonDown(1));
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
