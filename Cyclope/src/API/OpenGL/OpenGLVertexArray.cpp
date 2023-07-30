#include "OpenGLVertexArray.h"

namespace Cyclope {

	OpenGLVertexArray::OpenGLVertexArray(const Shared<VertexBuffer>& vertexBuffer, const Shared<IndexBuffer>& indexBuffer) {
		glGenVertexArrays(1, &m_ID);
		m_VertexBuffer = vertexBuffer;
		m_IndexBuffer = indexBuffer;
		Bind();
		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();
		//position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//texture attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
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