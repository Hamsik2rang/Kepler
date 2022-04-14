#pragma once

#include "Core/Base.h"
#include "Core/InputCode.h"

namespace kepler {

	struct KEPLER_API Axis
	{
		int horizontal = 0;
		int vertical = 0;
	};

	class KEPLER_API Input
	{
	private:
		static Axis m_axis;
		static uint64_t m_keyMap[5];
		
	public:
		static bool IsButtonDown(MouseCode mouseButton);
		static bool IsButtonUp(MouseCode mouseButton);
		static bool IsKeyDown(KeyCode keyCode);
		static bool IsKeyUp(KeyCode keyCode);

		static void SetKeyDown(KeyCode keyCode);
		static void SetKeyUp(KeyCode keyCode);
		static void SetButtonDown(MouseCode  mouseButton);
		static void SetButtonUp(MouseCode  mouseButton);

		inline static Axis GetAxis() { return m_axis; };
	};
}