#pragma once

#include "Event.hpp"

namespace kepler {
	
	class KEPLER_API KeyEvent : public Event
	{
	protected:
		int m_keyCode;

		KeyEvent(int keyCode)
			:m_keyCode(keyCode)
		{}

	public:
		inline int GetKeyCode() const { return m_keyCode; }
		
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT)
	};

	class KEPLER_API KeyPressedEvent : public KeyEvent
	{
	private:
		int m_repeatCount;
	
	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			:KeyEvent(keyCode), 
			m_repeatCount(repeatCount)
		{}

		inline int GetRepeatCount() const { return m_repeatCount; }
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode << "(" << m_repeatCount << " repeats )";

			return ss.str();
		}
		
		EVENT_CLASS_TYPE(KEY_PRESSED)
	};

	class KEPLER_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			:KeyEvent(keyCode)
		{}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_keyCode;

			return ss.str();
		}

		EVENT_CLASS_TYPE(KEY_RELEASED)
	};
}