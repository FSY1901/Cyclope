#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h"

#include "LayerStack.h"
#include "../ImGui/ImGuiLayer.h"
#include "Window.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

namespace Cyclope {

	class CYCLOPE_API Application {

	public:
		Application() = delete;
		Application(const WindowSpecification& spec);

		void Run();
		void PushLayer(Layer* layer);

		void OnEvent(Event& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnMouseMove(MouseMovedEvent& e); //for input class

		Window* GetWindow();

		static Application* GetInstance();

	private:

		static Application* m_Instance;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		Window m_window;
		Window win2;

		//Time
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;

	};

	//defined in app
	Cyclope::Application* CreateApplication();

}

#endif // !APPLICATION_H
