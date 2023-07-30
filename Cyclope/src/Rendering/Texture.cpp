#include "Texture.h"

#include "Rendering.h"
#include "../API/OpenGL/OpenGLTexture.h"

namespace Cyclope {

	Shared<Texture2D> Texture2D::Create(const char* path, bool flipped) {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path, flipped);
			break;
		}

	}

	Shared<Texture2D> Texture2D::Create(TextureSpecification spec, void* data) {
		
		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return  std::make_shared<OpenGLTexture2D>(spec, data);
			break;
		}

	}

}