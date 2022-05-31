#pragma once

#include "Core/Base.h"
#include "Core/InputCode.h"

namespace kepler {

	struct Axis
	{
		int horizontal = 0;
		int vertical = 0;
	};

	class Input
	{
	private:
		static Axis s_axis;
		static uint64_t s_keyMap[5];
		static const int s_bitCount;
		
	public:
		static bool IsButtonDown(MouseCode mouseButton);
		static bool IsButtonUp(MouseCode mouseButton);
		static bool IsKeyDown(KeyCode keyCode);
		static bool IsKeyUp(KeyCode keyCode);

		static void SetKeyDown(KeyCode keyCode);
		static void SetKeyUp(KeyCode keyCode);
		static void SetButtonDown(MouseCode  mouseButton);
		static void SetButtonUp(MouseCode  mouseButton);

		inline static Axis GetAxis() { return s_axis; };
	};
}