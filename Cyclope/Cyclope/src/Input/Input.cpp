#include "Input.h"

namespace Cyclope {

	GLFWwindow* Input::s_window = nullptr;

	float Input::s_mx = 0.0f;
	float Input::s_my = 0.0f;

	void Input::SetWindow(GLFWwindow* window) {
		s_window = window;
	}

	void Input::SetCursorMode(int mode)
	{
		glfwSetInputMode(s_window, GLFW_CURSOR, mode);
	}

	bool Input::KeyPressed(int key) {
		if (glfwGetKey(s_window, key) == GLFW_PRESS) {
			return true;
		}

		return false;
	}

	bool Input::ButtonPressed(int button)
	{
		if (glfwGetMouseButton(s_window, button) == GLFW_PRESS) {
			return true;
		}

		return false;
	}

	float Input::MouseX()
	{
		return s_mx;
	}

	float Input::MouseY()
	{
		return s_my;
	}

}