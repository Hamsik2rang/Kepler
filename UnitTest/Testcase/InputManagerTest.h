#pragma once

#include "Core/Input.h"

TEST(InputMappingTest1, InputTest)
{
	kepler::Input::SetKeyDown(kepler::key::A);
	EXPECT_TRUE(kepler::Input::IsButtonDown(kepler::key::A));
	kepler::Input::SetKeyUp(kepler::key::A);
}

TEST(InputMappingTest2, InputTest)
{
	EXPECT_FALSE(kepler::Input::IsButtonDown(kepler::key::B));
}

TEST(InputMappingTest3, InputTest)
{
	EXPECT_FALSE(kepler::Input::IsButtonDown(kepler::key::Left));
}

TEST(InputMappingTest4, InputTest)
{
	kepler::Input::SetKeyDown(kepler::key::Right);
	EXPECT_TRUE(kepler::Input::IsKeyDown(kepler::key::Right));
}