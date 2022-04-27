#pragma once

#include <sstream>

#include "Core\Event\Event.hpp"

namespace kepler {

	class MouseMovedEvent : public Event
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

	class MouseScrolledEvent : public Event
	{
	private:
		float m_vOffset;	// vertical
		float m_hOffset;	// horizontal. Most users don't have a mouse with an horizontal wheel.

	public:
		// Horizontal Scroll(hOffset)은 일반적인 마우스 조작의 경우 입력되지 않습니다.
		MouseScrolledEvent(float vOffset, float hOffset = 0.0f)
			:m_vOffset(vOffset), 
			m_hOffset(hOffset)
		{}

		inline float GetVOffset() const { return m_vOffset; }
		inline float GetHOffset() const { return m_hOffset; }
		
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_vOffset;

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	};

	class MouseButtonEvent : public Event
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


	class MouseButtonPressedEvent : public MouseButtonEvent
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

	class MouseButtonReleasedEvent : public MouseButtonEvent
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