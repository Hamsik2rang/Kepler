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
			:m_xPos(x),
			m_yPos(y)
		{}

		inline float GetX() const { return m_xPos; }
		inline float GetY() const { return m_yPos; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_xPos << ", " << m_yPos;
			
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	};

	class KEPLER_API MouseScrolledEvent : public Event
	{
	private:
		float m_zOffset;

	public:
		MouseScrolledEvent(float zOffset)
			:m_zOffset(zOffset)
		{}

		inline float GetZOffset() const { return m_zOffset; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_zOffset;

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
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

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)
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

		EVENT_CLASS_TYPE(MouseButtonPressed)
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

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}