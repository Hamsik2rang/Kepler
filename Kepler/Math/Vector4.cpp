#include "kepch.h"

#include "Vector4.hpp"

namespace kepler {

	const Vector4 Vector4::Zero		= { 0.0f, 0.0f, 0.0f, 0.0f };
	const Vector4 Vector4::Up		= { 0.0f, 1.0f, 0.0f, 0.0f };
	const Vector4 Vector4::Down		= { 0.0f, -1.0f, 0.0f, 0.0f };
	const Vector4 Vector4::Right	= { 1.0f, 0.0f, 0.0f, 0.0f };
	const Vector4 Vector4::Left		= { -1.0f, 0.0f, 0.0f, 0.0f };
	const Vector4 Vector4::Front	= { 0.0f, 0.0f, 1.0f, 0.0f };
	const Vector4 Vector4::Back		= { 0.0f, 0.0f, -1.0f, 0.0f };

}