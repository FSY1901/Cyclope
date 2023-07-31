#include "OpenGLVertexArray.h"

namespace Cyclope {

	OpenGLVertexArray::OpenGLVertexArray(const Shared<VertexBuffer>& vertexBuffer, const Shared<IndexBuffer>& indexBuffer) {
		glGenVertexArrays(1, &m_ID);
		m_VertexBuffer = vertexBuffer;
		m_IndexBuffer = indexBuffer;
		Bind();
		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();
		m_VertexBuffer->LinkVertexAttributes();
		m_VertexBuffer->Unbind();
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() const {
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	const Shared<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() {
		return m_IndexBuffer;
	}

}