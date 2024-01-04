#include "Cpch.h"
#include "Texture.h"

#include "Rendering.h"
#include "../API/OpenGL/OpenGLTexture.h"
#include "stb_image.h"

namespace Cyclope {

	Shared<Texture2D> Texture2D::Create(const char* path) {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			TextureSpecification spec;
			// load and generate the texture
			int nrChannels;
			stbi_set_flip_vertically_on_load(true);
			//for pngs
			spec.Format = ImageFormat::RGBA8;
			unsigned char* data = stbi_load(path, &spec.Width, &spec.Height, &nrChannels, STBI_rgb_alpha); //unsigned char*
			if (!data) {
				CYCLOPE_CORE_WARN("Failed to load texture from path: " + std::string(path));
			}
			return std::make_shared<OpenGLTexture2D>(spec, data);
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