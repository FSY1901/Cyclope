#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h"

#include "LayerStack.h"
#include "../ImGui/ImGuiLayer.h"
#include "Window.h"

namespace Cyclope {

	struct CYCLOPE_API WindowSpecification {
		int width, height = 0;
		bool maximized = false; //change to window mode (fullscreen, maximized, windowed)
		const char* title = "";
	};

	class CYCLOPE_API Application {

	public:
		Application() = delete;
		Application(const WindowSpecification& spec);

		void Run();
		void PushLayer(Layer* layer);

		Window* GetWindow();

		static Application* GetInstance();

	private:

		static Application* m_Instance;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		Window m_window;

		//Time
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;

	};

	//defined in app
	Cyclope::Application* CreateApplication();

}

#endif // !APPLICATION_H
