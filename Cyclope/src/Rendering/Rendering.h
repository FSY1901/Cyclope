/*
This file is used for various rendering commands that can be called by the user
*/

#include "Core.h"

#include "glad.h"
#include "glfw3.h"

#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"

#ifndef RENDERING_H
#define RENDERING_H

namespace Cyclope {

	enum class RendererAPI {
		OpenGL
	};
	
	enum class CYCLOPE_API DrawMode {
		Point = GL_POINT,
		Line = GL_LINE,
		Fill = GL_FILL
	};

	struct CYCLOPE_API RenderCommands
	{
		static void SetClearColor(float r, float g, float b);
		static void Clear();
		static void SetDrawMode(DrawMode mode);
	};

	class CYCLOPE_API Renderer {
	public:
		static void BeginScene();
		static void EndScene();
		static void Submit(const Shared<VertexArray>& vertexArray, const Shader& shader);

		static RendererAPI GetAPI();
	private:
		static RendererAPI s_RendererAPI;
	};

}

#endif // !RENDERING_H