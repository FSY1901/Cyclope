#pragma once

#include "Core.h"
#include "glad.h"
#include "../Rendering/Buffer.h"

#include <memory>

namespace Cyclope {

	class CYCLOPE_API OpenGLVertexBuffer : public VertexBuffer{

	public:
		OpenGLVertexBuffer(float* vertices, GLsizeiptr size, const BufferLayout& layout);
		~OpenGLVertexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

		void SetBufferLayout(const BufferLayout& layout) override;
		void LinkVertexAttributes() override;

		GLsizeiptr GetVertexCount() const override;

	private:
		unsigned int m_ID;
		BufferLayout m_layout;
		GLsizeiptr m_vertexCount;

	};

	class CYCLOPE_API OpenGLIndexBuffer : public IndexBuffer {

	public:
		OpenGLIndexBuffer(unsigned int* indices, GLsizeiptr size);
		~OpenGLIndexBuffer() override;

		void Bind() const override;
		void Unbind() const override;
		
		GLsizeiptr GetSize() const override;

	private:
		unsigned int m_ID;
		GLsizeiptr m_size;

	};

}