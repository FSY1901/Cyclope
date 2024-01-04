#include "Cpch.h"

#include "VertexArray.h"
#include "Rendering.h"
#include "../API/OpenGL/OpenGLVertexArray.h"

namespace Cyclope {

	Shared<VertexArray> VertexArray::Create(const Shared<VertexBuffer>& vertexBuffer, const Shared<IndexBuffer>& indexBuffer) {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return std::make_shared<OpenGLVertexArray>(vertexBuffer, indexBuffer);
			break;
		}

	}

}