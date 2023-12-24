#pragma once

#include "Core.h"

#include "glad.h"
#include "glfw3.h"

namespace Cyclope {

	struct WindowSpecification;

	class CYCLOPE_API Window {

	public:
		Window() {};

		void Create(const WindowSpecification& spec);
		void Update();
		void SetWindowTitle(const char* title);

		GLFWwindow* GetWindow() { return m_window; }
		int GetWidth() { return m_width; }
		int GetHeight() { return m_height; }
		float GetAspectRatio() { return (float)m_width / (float)m_height; }
		const char* GetTitle() { return m_title; }

		bool IsMinimized() { return m_width == 0 && m_height == 0; }

	private:
		GLFWwindow* m_window;
		int m_width, m_height;
		const char* m_title;

		//CALLBACKS
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		friend class Application;

	};

}