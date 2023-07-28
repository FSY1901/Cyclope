#include "Time.h"

#include "glfw3.h"

namespace Cyclope {

	double Time::GetTime() {
		return glfwGetTime();
	}

}