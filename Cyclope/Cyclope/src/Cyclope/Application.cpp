#include "Application.h"

#include <iostream>

#include "../Game/Scene.h"
#include "../Input/Input.h"

namespace Cyclope {

	Application::Application() {

	}

    Application::Application(int width, int height, const char* title) {
        m_window.width = width;
        m_window.height = height;
        m_window.title = title;
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

        return 0;
    }

	void Application::Run() {

        int err = Init();

        if (err == -1) {
            std::cout << "ERROR HAPPENED WHEN TRYING TO RUN APPLICATION";
            return;
        }

        Input::SetWindow(m_window.window);

        IMGUI_CHECKVERSION();
        ctx = ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window.window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        Start();

        while (!glfwWindowShouldClose(m_window.window))
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            Update();

            ImGuiUpdate();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(m_window.window);
            glfwPollEvents();
        }

        glfwTerminate();

	}

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