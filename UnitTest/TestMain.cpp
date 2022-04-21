#include "kepch.h"
#include "pch.h"

//#include "./Testcase/Vector4DTest.h"
//#include "./Testcase/Vector3DTest.h"
//#include "./Testcase/Vector2DTest.h"

#include "./Testcase/Matrix22Test.h"


int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
