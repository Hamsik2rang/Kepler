#pragma once

#include <functional>
#include <string>
#include <iostream>

#include "Core/Base.h"

namespace kepler {
	
	enum class EEventType
	{
		None = 0,

		KeyPressed,
		KeyReleased,

		MouseMoved,
		MouseScrolled,
		MouseButtonPressed,
		MouseButtonReleased,

		WindowClosed,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,

		AppTick,
		AppUpdate,
		AppRender,
	};

	enum EventCategory
	{
		None						= 0,		// 0b 0000 0000
		EventCategoryApplication	= BIT(0),	// 0b 0000 0001
		EventCategoryInput			= BIT(1),	// 0b 0000 0010
		EventCategoryKeyboard		= BIT(2),	// 0b 0000 0100
		EventCategoryMouse			= BIT(3),	// 0b 0000 1000
		EventCategoryMouseButton	= BIT(4),	// 0b 0001 0000
	};

#define EVENT_CLASS_TYPE(type) static EEventType GetStaticType() { return EEventType::##type; }\
								virtual EEventType GetEventType() const override {return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
		friend class EventDispatcher;

	protected:
		bool m_bIsHandled = false;

	public:
		virtual EEventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return std::string(GetName()); }

		inline bool IsHandled() const { return m_bIsHandled; };
		inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }
	};

	class EventDispatcher
	{
		template <typename T>
		using EventFunc = std::function<bool(T&)>;
	private:
		Event& m_event;
	
	public:
		EventDispatcher(Event& event)
			: m_event(event)
		{}

		template<typename T>
		bool Dispatch(EventFunc<T> func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.m_bIsHandled = func(*(T*)&m_event);
				return true;
			}

			return false;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
