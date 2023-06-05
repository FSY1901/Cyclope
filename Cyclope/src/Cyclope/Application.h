#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h"

#include "LayerStack.h"
#include "../ImGui/ImGuiLayer.h"

#include "glad.h"
#include "glfw3.h"

namespace Cyclope {

	struct CYCLOPE_API Window
	{
		GLFWwindow* window;
		int width;
		int height;
		const char* title;
	};

	class CYCLOPE_API Application {

	public:
		Application() = delete;
		Application(int width, int height, const char* title);

		void Run();
		void PushLayer(Layer* layer);

		static Application* GetInstance();

		Window GetWindow();

	protected:

		int GetWindowWidth();
		int GetWindowHeight();

		const char* GetWindowTitle();
		void SetWindowTitle(const char* title);

	private:

		static Application* m_Instance;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

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
