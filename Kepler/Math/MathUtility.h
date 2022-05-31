#pragma once

#include "Constant.h"

namespace kepler {
	namespace math {

		inline float DegToRad(float degree)
		{
			// 180' = pi
			// 1' = pi/180
			// deg = pi * deg / 180
			return constant::PI * degree / 180.0f;
		}

		inline float RadToDeg(float radian)
		{
			// pi(rad) = 180'
			// 1(rad) = 180' / pi
			// radian(rad) = 180' * radian / pi
			return radian * 180.0f / constant::PI;
		}
	}
}