#include "Input.h"

namespace Cyclope {

#pragma region Static Variables
	GLFWwindow* Input::s_window = nullptr;

	float Input::s_mx = 0.0f;
	float Input::s_my = 0.0f;
#pragma endregion

	void Input::SetWindow(GLFWwindow* window) {
		s_window = window;
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		glfwSetInputMode(s_window, GLFW_CURSOR, (int)mode);
	}

	bool Input::KeyPressed(Key key) {
		if (glfwGetKey(s_window, (int)key) == GLFW_PRESS) {
			return true;
		}

		return false;
	}

	bool Input::ButtonPressed(Button button){
		if (glfwGetMouseButton(s_window, (int)button) == GLFW_PRESS) {
			return true;
		}

		return false;
	}

	float Input::MouseX() { return s_mx; }

	float Input::MouseY() { return s_my; }

}