#include "Application.h"

#include <iostream>

#include "../Game/Scene.h"
#include "../Input/Input.h"

namespace Cyclope {

    Application* Application::m_Instance = nullptr;

    Application::Application(int width, int height, const char* title) {
        m_window.width = width;
        m_window.height = height;
        m_window.title = title;
        m_Instance = this;

        Init();
    }

    int Application::Init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        GLFWwindow* window = glfwCreateWindow(m_window.width, m_window.height, m_window.title, NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        m_window.window = window;
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        m_ImGuiLayer = new ImGuiLayer();
        PushLayer(m_ImGuiLayer);

        return 0;
    }

	void Application::Run() {

        Input::SetWindow(m_window.window);

        while (!glfwWindowShouldClose(m_window.window))
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

            glfwSwapBuffers(m_window.window);
            glfwPollEvents();
        }

        glfwTerminate();

	}

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    Application* Application::GetInstance() { return m_Instance; }
    
    Window Application::GetWindow() { return m_window; }

    int Application::GetWindowWidth() { return m_window.width; }
    int Application::GetWindowHeight() { return m_window.height; }

    const char* Application::GetWindowTitle() { return m_window.title; }
    void Application::SetWindowTitle(const char* title) {
        m_window.title = title;
        glfwSetWindowTitle(m_window.window, title);
    }

    void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        Input::s_mx = xpos;
        Input::s_my = ypos;
    }

}