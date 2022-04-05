#pragma once

#include <sstream>

#include "Core\Event\Event.hpp"

namespace kepler {

	class KEPLER_API MouseMovedEvent : public Event
	{
	private:
		float m_xPos;
		float m_yPos;

	public:
		MouseMovedEvent(float x, float y)
			:m_xPos(x), m_yPos(y)
		{}

		inline float GetX() const { return m_xPos; }
		inline float GetY() const { return m_yPos; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_xPos << ", " << m_yPos;
			
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_MOVED)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)
	};

	class KEPLER_API MouseScrolledEvent : public Event
	{
	private:
		float m_xOffset;
		float m_yOffset;

	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			:m_xOffset(xOffset), m_yOffset(yOffset)
		{}

		inline float GetXOffset() const { return m_xOffset; }
		inline float GetYOffset() const { return m_yOffset; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_xOffset << ", " << m_yOffset;

			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_SCROLLED)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)
	};

	class KEPLER_API MouseButtonEvent : public Event
	{
	protected:
		int m_button;

		MouseButtonEvent(int button)
			:m_button(button)
		{}
	public:
		inline int GetMouseButton() const { return m_button; }

		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSEBUTTON | EVENT_CATEGORY_INPUT)
	};


	class KEPLER_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			:MouseButtonEvent(button)
		{}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_button;
			
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
	};

	class KEPLER_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			:MouseButtonEvent(button)
		{}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_button;

			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED)
	};
}