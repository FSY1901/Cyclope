#include "Shader.h"

#include "Rendering.h"
#include "../API/OpenGL/OpenGLShader.h"

namespace Cyclope {

    Shared<Shader> Shader::Create(std::string path) {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return std::make_shared<OpenGLShader>(path);
			break;
		}

    }

}