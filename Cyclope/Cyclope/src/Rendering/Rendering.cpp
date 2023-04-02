#include "Rendering.h"

namespace Cyclope {

	void Renderer::ClearColor(float r, float g, float b) {
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

}