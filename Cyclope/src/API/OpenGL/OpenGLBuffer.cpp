#pragma once

#include "OpenGLBuffer.h"

namespace Cyclope {

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, GLsizeiptr size, const BufferLayout& layout) {
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		SetBufferLayout(layout);
		m_vertexCount = size / layout.GetOffset(); //how many floats make up one vertex
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
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

	void OpenGLVertexBuffer::SetBufferLayout(const BufferLayout& layout) { m_layout = layout; }
	
	void OpenGLVertexBuffer::LinkVertexAttributes() {

		int i = 0;
		for (const auto& element : m_layout.GetElements())
		{
			switch (element.type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glVertexAttribPointer(i,
					element.GetComponentCount(),
					GL_FLOAT,
					element.normalized ? GL_TRUE : GL_FALSE,
					m_layout.GetStride(),
					(void*)element.offset);
				glEnableVertexAttribArray(i);
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glVertexAttribIPointer(i,
					element.GetComponentCount(),
					GL_INT,
					m_layout.GetStride(),
					(const void*)element.offset);
				glEnableVertexAttribArray(i);
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glVertexAttribPointer(i,
						count,
						GL_FLOAT,
						element.normalized ? GL_TRUE : GL_FALSE,
						m_layout.GetStride(),
						(const void*)(element.offset + sizeof(float) * count * i));
					glVertexAttribDivisor(i, 1);
					glEnableVertexAttribArray(i);
				}
				break;
			}
			}
			i++;
		}

	}

	GLsizeiptr OpenGLVertexBuffer::GetVertexCount() const
	{
		return m_vertexCount;
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, GLsizeiptr size) {
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		m_size = size * sizeof(unsigned int);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, indices, GL_STATIC_DRAW);
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

}