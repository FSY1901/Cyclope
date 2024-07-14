#pragma once

#include "Event.h"

namespace Cyclope {

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(unsigned int x, unsigned int y)
			: m_x(x), m_y(y) {}

		unsigned int GetWidth() const { return m_x; }
		unsigned int GetHeight() const { return m_y; }

		EVENT_CLASS_TYPE(WindowMoved)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_x, m_y;
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() = default;

		EVENT_CLASS_TYPE(WindowFocus)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() = default;

		EVENT_CLASS_TYPE(WindowLostFocus)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

/*
	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
*/
}