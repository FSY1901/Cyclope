#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h"

#include "LayerStack.h"
#include "../ImGui/ImGuiLayer.h"
#include "Window.h"

namespace Cyclope {

	class CYCLOPE_API Application {

	public:
		Application() = delete;
		Application(int width, int height, const char* title);

		void Run();
		void PushLayer(Layer* layer);

		static Application* GetInstance();

		Window* GetWindow();

	private:

		static Application* m_Instance;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		Window m_window;

		void Init();

	};

	//defined in app
	Application* CreateApplication();

}

#endif // !APPLICATION_H
