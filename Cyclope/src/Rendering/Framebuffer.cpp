#include "Framebuffer.h"
#include "../API/OpenGL/OpenGLFramebuffer.h"
#include "Rendering.h"

namespace Cyclope {

	Shared<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specs) {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return std::make_shared<OpenGLFramebuffer>(specs);
			break;
		}

	}

}