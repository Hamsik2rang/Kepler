#pragma once

#include "../Core.h"

namespace kepler {
	// 현재 사용은 불가능함.
	enum class eEventType
	{
		NONE = 0,

		WINDOW_CLOSE,
		WINDOW_RESIZE,
		WINDOW_FOCUS,
		WINDOW_LOST_FOCUS,
		WINDOW_MOVED,

		APP_TICK,
		APP_UPDATE,
		APP_RENDER,

		KEY_PRESSED,
		KEY_RELEASED,

		MOUSE_BUTTON_PRESSED,
		MOUSE_BUTTON_RELEASED,
		MOUSE_MOVED,
		MOUSE_SCROLLED,
	};

	enum eEventCategory
	{
		NONE						= 0,		// 0x00000000
		EVENT_CATEGORY_APPLICATION	= BIT(0),	// 0x00000001
		EVENT_CATEGORY_INPUT		= BIT(1),	// 0x00000010
		EVENT_CATEGORY_KEYBOARD		= BIT(2),	// 0x00000100
		EVENT_CATEGORY_MOUSE		= BIT(3),	// 0x00001000
		EVENT_CATEGORY_MOUSEBUTTON	= BIT(4),	// 0x00010000
	};

#define EVENT_CLASS_TYPE(type) static eEventType GetStaticType() { return eEventType::##type;}\
								virtual eEventType GetEventType() const override {return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class KEPLER_API Event
	{
		friend class EventDispatcher;

	protected:
		bool m_bIsHandled = false;

	public:
		virtual eEventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return std::string(GetName()); }

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