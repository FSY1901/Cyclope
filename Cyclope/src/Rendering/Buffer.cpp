#pragma once

#include "Buffer.h"

#include "Rendering.h"
#include "../API/OpenGL/OpenGLBuffer.h"

namespace Cyclope {

	void BufferLayout::CalculateOffsetsAndStride() {
		int offset = 0;

		for (auto& elem : m_elements) {
			elem.offset = offset;
			offset += elem.size;
			m_stride += elem.size;
		}
	}

	Shared<VertexBuffer> VertexBuffer::Create(float* vertices, GLsizeiptr size) {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(vertices, size);
			break;
		}

	}

	Shared<IndexBuffer> IndexBuffer::Create(unsigned int* indices, GLsizeiptr size) {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indices, size);
			break;
		}

	}

}