#pragma once

#include "Core/Base.h"
#include "Core/Event/Event.hpp"


namespace kepler {

	// Window Properties
	struct WindowProperty
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProperty(const std::string& _title = "Kepler Engine", uint32_t _width = 1280, uint32_t _height = 720)
			:title(_title),
			 width(_width),
			 height(_height)
		{}
	};
	
	// Interface for platform window abstraction
	class IWindow
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~IWindow() {}
		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;
		virtual void SetVSync(bool isEnabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual HWND GetWindowHandle() const = 0;

		static IWindow* Create(const WindowProperty& props = WindowProperty());

	};

}