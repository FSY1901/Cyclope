#include "Cpch.h"

#include "Application.h"
#include "../Game/Scene.h"
#include "../Input/Input.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Cyclope {

    Application* Application::m_Instance = nullptr;

    Application::Application(const WindowSpecification& spec) {
        Cyclope::Log::Init();

        CYCLOPE_CORE_ASSERT(!m_Instance, "Application Instance already exists");
        m_Instance = this;

        m_window.Create(spec);
        m_window.SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushLayer(m_ImGuiLayer);

        Input::SetWindow(m_window.m_window);
    }

    void Application::Run() {
        //glEnable(GL_MULTISAMPLE);
        while (!glfwWindowShouldClose(m_window.m_window))
        {
            float currentTime = glfwGetTime();
            deltaTime = currentTime - lastFrame;
            lastFrame = currentTime;

            if (!m_window.IsMinimized()) {
                {
                    for (Layer* layer : m_LayerStack)
                        layer->OnUpdate(deltaTime);
                }

                m_ImGuiLayer->Begin();
                {
                    for (Layer* layer : m_LayerStack)
                        layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }

            m_window.Update();
        }

        glfwTerminate();

	}

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Application::OnMouseMove));
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    bool Application::OnWindowResize(WindowResizeEvent& e){
        glViewport(0, 0, e.GetWidth(), e.GetHeight());
        return true;
    }

    bool Application::OnMouseMove(MouseMovedEvent& e) {
        Input::s_mx = e.GetX();
        Input::s_my = e.GetY();
        return true;
    }

    Application* Application::GetInstance() { return m_Instance; }
    
    Window* Application::GetWindow() { return &m_window; }

}