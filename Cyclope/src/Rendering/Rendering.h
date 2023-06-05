/*
This file is used for various rendering commands that can be called by the user
*/

#include "Core.h"

#include "glad.h"
#include "glfw3.h"

#ifndef RENDERING_H
#define RENDERING_H

namespace Cyclope {
	
	struct CYCLOPE_API Renderer
	{
		static void ClearColor(float r, float g, float b);
	};

}

#endif // !RENDERING_H