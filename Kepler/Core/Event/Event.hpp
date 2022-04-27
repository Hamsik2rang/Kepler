#pragma once

#include <functional>
#include <string>
#include <iostream>

#include "Core/Base.h"

namespace kepler {
	
	enum class eEventType
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

	enum eEventCategory
	{
		None						= 0,		// 0x00000000
		EventCategoryApplication	= BIT(0),	// 0x00000001
		EventCategoryInput			= BIT(1),	// 0x00000010
		EventCategoryKeyboard		= BIT(2),	// 0x00000100
		EventCategoryMouse			= BIT(3),	// 0x00001000
		EventCategoryMouseButton	= BIT(4),	// 0x00010000
	};

#define EVENT_CLASS_TYPE(type) static eEventType GetStaticType() { return eEventType::##type; }\
								virtual eEventType GetEventType() const override {return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
		friend class EventDispatcher;

	protected:
		bool m_bIsHandled = false;

	public:
		virtual eEventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return std::string(GetName()); }

		inline bool IsHandled() const { return m_bIsHandled; };
		inline bool IsInCategory(eEventCategory category) { return GetCategoryFlags() & category; }
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
