#pragma once

#include "Core.h"

#include "glad.h"
#include "glfw3.h"

namespace Cyclope {

	class CYCLOPE_API Window {

	public:
		Window() {};
		Window(const char* title, int width, int height);

		int Create();
		void Update();
		void SetWindowTitle(const char* title);

		GLFWwindow* GetWindow() { return m_window; }
		int GetWidth() { return m_width; }
		int GetHeight() { return m_height; }
		const char* GetTitle() { return m_title; }

	private:
		GLFWwindow* m_window;
		int m_width, m_height;
		const char* m_title;

		//CALLBACKS
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

		friend class Application;

	};

}