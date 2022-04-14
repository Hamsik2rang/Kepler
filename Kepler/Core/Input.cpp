#include "kepch.h"

#include "Input.h"

namespace kepler {

	const int BIT_UINT64 = 64;

	Axis Input::m_axis{};
	uint64_t Input::m_keyMap[5]{};

	bool Input::IsButtonDown(MouseCode mouseButton)
	{
		KEPLER_ASSERT(mouseButton < 256, "invalid button");
		return m_keyMap[mouseButton / BIT_UINT64] & BIT(mouseButton % BIT_UINT64);
	}

	bool Input::IsButtonUp(MouseCode mouseButton)
	{
		KEPLER_ASSERT(mouseButton < 256, "invalid button");
		return !(m_keyMap[mouseButton / BIT_UINT64] & BIT(mouseButton % BIT_UINT64));
	}

	bool Input::IsKeyDown(KeyCode keyCode)
	{
		KEPLER_ASSERT(keyCode < 256, "invalid keycode");
		return m_keyMap[keyCode / BIT_UINT64] & BIT(keyCode % BIT_UINT64);
	}

	bool Input::IsKeyUp(KeyCode keyCode)
	{
		KEPLER_ASSERT(keyCode < 256, "invalid keycode");
		return !(m_keyMap[keyCode / BIT_UINT64] & BIT(keyCode % BIT_UINT64));
	}

	void Input::SetKeyDown(KeyCode keyCode)
	{
		m_keyMap[keyCode / BIT_UINT64] |= BIT(keyCode % BIT_UINT64);
	}

	void Input::SetKeyUp(KeyCode keyCode)
	{
		m_keyMap[keyCode / BIT_UINT64] &= ~BIT(keyCode % BIT_UINT64);
	}

	void Input::SetButtonDown(MouseCode mouseButton)
	{
		m_keyMap[mouseButton / BIT_UINT64] |= BIT(mouseButton % BIT_UINT64);
	}

	void Input::SetButtonUp(MouseCode mouseButton)
	{
		m_keyMap[mouseButton / BIT_UINT64] &= ~BIT(mouseButton % BIT_UINT64);
	}
}