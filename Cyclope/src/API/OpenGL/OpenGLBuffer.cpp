#pragma once

#include "OpenGLBuffer.h"

namespace Cyclope {

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, GLsizeiptr size) {
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLVertexBuffer::Bind() const{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void OpenGLVertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, GLsizeiptr size) {
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		m_size = size;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLIndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void OpenGLIndexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLsizeiptr OpenGLIndexBuffer::GetSize() const { return m_size; }

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

	void OpenGLVertexArray::Bind() const{
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Unbind() const{
		glBindVertexArray(0);
	}

	const Shared<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() {
		return m_IndexBuffer;
	}

}