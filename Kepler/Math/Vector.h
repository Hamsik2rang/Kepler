#pragma once

// float, int, char, uchar만을 받도록 template 인자를 제한합니다.
#define TYPE_CONSTRAINT_FLOAT_INT_CHAR_UCHAR(T) template <typename T, typename =\
			std::enable_if<\
			std::is_same<float, T>::value || std::is_same<int, T>::value || std::is_same<char, T>::value || std::is_same<unsigned char, T>::value>>

#include "Vector2D.hpp"
#include "Vector3D.hpp"
#include "Vector4D.hpp"
