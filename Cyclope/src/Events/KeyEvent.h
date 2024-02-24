#pragma once

#include "Input/Input.h"
#include "Event.h"

namespace Cyclope {

	class KeyEvent : public Event
	{
	public:
		Key GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(const Key keycode)
			: m_KeyCode(keycode) {}

		Key m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const Key keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		bool IsRepeat() const { return m_IsRepeat; }

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_IsRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const Key keycode)
			: KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const Key keycode)
			: KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}