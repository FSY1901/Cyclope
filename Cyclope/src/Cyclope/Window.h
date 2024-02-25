#pragma once

#include "Core.h"

#include "Events/Event.h"

#include "glad.h"
#include "glfw3.h"

namespace Cyclope {

	struct CYCLOPE_API WindowSpecification {
		int width, height = 0;
		bool maximized = false; //change to window mode (fullscreen, maximized, windowed)
		const char* title = "";
	};

	//TODO: add VSync option
	class CYCLOPE_API Window {

	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window() {};

		void Create(const WindowSpecification& spec);

		void Update();

		GLFWwindow* GetGLFWWindow() { return m_window; }
		int GetWidth() { return m_data.m_width; }
		int GetHeight() { return m_data.m_height; }
		float GetAspectRatio() { return (float)m_data.m_width / (float)m_data.m_height; }
		const char* GetTitle() { return m_data.m_title; }
		bool IsMinimized() { return m_data.m_width == 0 && m_data.m_height == 0; }

		void SetWindowTitle(const char* title);
		void SetEventCallback(const EventCallbackFn& callback) { m_data.EventCallback = callback; }

	private:
		GLFWwindow* m_window;

		struct WindowData {
			int m_width, m_height;
			const char* m_title;
			EventCallbackFn EventCallback;
		};

		WindowData m_data;

		friend class Application;

	};

}