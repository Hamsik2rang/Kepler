#include "kepch.h"

#include "Input.h"

namespace kepler {

	Axis Input::s_axis{};
	uint64_t Input::s_keyMap[5]{};
	const int Input::s_bitCount = 64;

	bool Input::IsButtonDown(MouseCode mouseButton)
	{
		KEPLER_ASSERT(mouseButton < 256, "invalid button");

		return s_keyMap[mouseButton / s_bitCount] & BIT_UINT64(mouseButton % s_bitCount);
	}

	bool Input::IsButtonUp(MouseCode mouseButton)
	{
		KEPLER_ASSERT(mouseButton < 256, "invalid button");

		return !(s_keyMap[mouseButton / s_bitCount] & BIT_UINT64(mouseButton % s_bitCount));
	}

	bool Input::IsKeyDown(KeyCode keyCode)
	{
		KEPLER_ASSERT(keyCode < 256, "invalid keycode");

		return s_keyMap[keyCode / s_bitCount] & BIT_UINT64(keyCode % s_bitCount);
	}

	bool Input::IsKeyUp(KeyCode keyCode)
	{
		KEPLER_ASSERT(keyCode < 256, "invalid keycode");

		return !(s_keyMap[keyCode / s_bitCount] & BIT_UINT64(keyCode % s_bitCount));
	}

	void Input::SetKeyDown(KeyCode keyCode)
	{
		s_keyMap[keyCode / s_bitCount] |= BIT_UINT64(keyCode % s_bitCount);
	}

	void Input::SetKeyUp(KeyCode keyCode)
	{
		s_keyMap[keyCode / s_bitCount] &= ~BIT_UINT64(keyCode % s_bitCount);
	}

	void Input::SetButtonDown(MouseCode mouseButton)
	{
		s_keyMap[mouseButton / s_bitCount] |= BIT_UINT64(mouseButton % s_bitCount);
	}

	void Input::SetButtonUp(MouseCode mouseButton)
	{
		s_keyMap[mouseButton / s_bitCount] &= ~BIT_UINT64(mouseButton % s_bitCount);
	}
}