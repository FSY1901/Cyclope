#include "Cpch.h"
#include "Window.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Application.h"
#include "../Input/Input.h"

#include <iostream>

#include "Application.h"
#include "Log.h"

namespace Cyclope {

	void Window::Create(const WindowSpecification& spec) {

        m_data.m_width = spec.width;
        m_data.m_height = spec.height;
        m_data.m_title = spec.title;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if(spec.maximized)
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

        GLFWwindow* window = glfwCreateWindow(m_data.m_width, m_data.m_height, m_data.m_title, NULL, NULL);
        if (!window) {
            glfwTerminate();
        }
        CYCLOPE_CORE_ASSERT(window, "Failed to create Window");
        m_window = window;
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(0);//disable vsync
        glfwSetWindowUserPointer(m_window, &m_data);
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.m_width = width;
                data.m_height = height;
                WindowResizeEvent resizeEvent(width, height);
                data.EventCallback(resizeEvent);
            });
        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data.EventCallback(event);
            });
        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                switch (action) {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event((Key)key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event((Key)key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event((Key)key, 1);
                    data.EventCallback(event);
                    break;
                }
                }
            });
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action) {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event((Button)button);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event((Button)button);
                data.EventCallback(event);
                break;
            }
            }
            });
        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)xoffset, (float)yoffset);
            data.EventCallback(event);
            });
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)xpos, (float)ypos);
            data.EventCallback(event);
            });

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            CYCLOPE_CORE_ASSERT(0, "Failed to initialize GLAD");
            return;
        }

	}

    void Window::Update() {
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    void Window::SetWindowTitle(const char* title) {
        m_data.m_title = title;
        glfwSetWindowTitle(m_window, title);
    }

    /*void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        Application::GetInstance()->GetWindow()->m_data.m_width = width;
        Application::GetInstance()->GetWindow()->m_data.m_height = height;

        glViewport(0, 0, width, height);
    }

    void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
        Input::s_mx = xpos;
        Input::s_my = ypos;
    }*/

}