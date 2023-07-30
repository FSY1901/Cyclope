#include "Application.h"

#include <iostream>

#include "../Game/Scene.h"
#include "../Input/Input.h"

namespace Cyclope {

    Application* Application::m_Instance = nullptr;

    Application::Application(int width, int height, const char* title) {
        m_window = Window(title, width, height);
        m_Instance = this;

        Init();
    }

    void Application::Init() {
        int error = m_window.Create();

        if (error == -1)
            return;

        m_ImGuiLayer = new ImGuiLayer();
        PushLayer(m_ImGuiLayer);
    }

	void Application::Run() {

        Input::SetWindow(m_window.m_window);

        while (!glfwWindowShouldClose(m_window.m_window))
        {
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate();
            }

            m_ImGuiLayer->Begin();
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            m_window.Update();
        }

        glfwTerminate();

	}

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    Application* Application::GetInstance() { return m_Instance; }
    
    Window* Application::GetWindow() { return &m_window; }

}