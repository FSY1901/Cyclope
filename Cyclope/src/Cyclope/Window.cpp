#include "Window.h"
#include "Application.h"
#include "../Input/Input.h"

#include <iostream>

#include "Application.h"

namespace Cyclope {

	void Window::Create(const WindowSpecification& spec) {

        m_width = spec.width;
        m_height = spec.height;
        m_title = spec.title;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if(spec.maximized)
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

        GLFWwindow* window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        m_window = window;
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(0);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        glfwSetCursorPosCallback(m_window, cursor_pos_callback);
        glfwSetScrollCallback(m_window, scroll_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }

	}

    void Window::Update() {
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    void Window::SetWindowTitle(const char* title) {
        m_title = title;
        glfwSetWindowTitle(m_window, title);
    }

    void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        Application::GetInstance()->GetWindow()->m_width = width;
        Application::GetInstance()->GetWindow()->m_height = height;

        glViewport(0, 0, width, height);
    }

    void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
        Input::s_mx = xpos;
        Input::s_my = ypos;
    }

    void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        //TODO: find solution to scrolling
    }

}