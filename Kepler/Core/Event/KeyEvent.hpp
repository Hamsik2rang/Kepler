#pragma once

#include <sstream>

#include "Core\Event\Event.hpp"

namespace kepler {
	
	class KeyEvent : public Event
	{
	protected:
		int m_keyCode;

		KeyEvent(int keyCode)
			:m_keyCode(keyCode)
		{}

	public:
		inline int GetKeyCode() const { return m_keyCode; }
		
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	};

	class KeyPressedEvent : public KeyEvent
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
			ss << "KeyPressedEvent: " << m_keyCode << "(" << m_repeatCount << " repeats)";

			return ss.str();
		}
		
		EVENT_CLASS_TYPE(KeyPressed)
	};

	class KeyReleasedEvent : public KeyEvent
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

		EVENT_CLASS_TYPE(KeyReleased)
	};
}