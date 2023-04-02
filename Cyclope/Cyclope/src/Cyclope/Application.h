#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glad.h"
#include "glfw3.h"

namespace Cyclope {

	struct API Window
	{
		GLFWwindow* window;
		int width;
		int height;
		const char* title;
	};

	class API Application {

	public:
		Application();
		Application(int width, int height, const char* title);

		void Run();

		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void ImGuiUpdate() = 0;

	protected:
		ImGuiContext* ctx;

		int GetWindowWidth();
		int GetWindowHeight();

		const char* GetWindowTitle();
		void SetWindowTitle(const char* title);

	private:

		Window m_window;

		int Init();

		//CALLBACKS
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	};

	//defined in app
	Application* CreateApplication();

}

#endif // !APPLICATION_H
