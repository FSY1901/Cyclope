#pragma once

#include "Core.h"
#include "glad.h"
#include "../Rendering/Buffer.h"

#include <memory>

namespace Cyclope {

	class CYCLOPE_API OpenGLVertexBuffer : public VertexBuffer{

	public:
		OpenGLVertexBuffer(float* vertices, GLsizeiptr size);
		virtual ~OpenGLVertexBuffer();

		void Bind() const;
		void Unbind() const;

	private:
		unsigned int m_ID;

	};

	class CYCLOPE_API OpenGLIndexBuffer : public IndexBuffer {

	public:
		OpenGLIndexBuffer(unsigned int* indices, GLsizeiptr size);
		virtual ~OpenGLIndexBuffer();

		void Bind() const;
		void Unbind() const;
		
		GLsizeiptr GetSize() const;

	private:
		unsigned int m_ID;
		GLsizeiptr m_size;

	};

	class CYCLOPE_API OpenGLVertexArray : public VertexArray {

	public:
		OpenGLVertexArray(const Shared<VertexBuffer>& vertexBuffer, const Shared<IndexBuffer>& indexBuffer);
		virtual ~OpenGLVertexArray();

		void Bind() const;
		void Unbind() const;

		const Shared<IndexBuffer>& GetIndexBuffer();

	private:
		unsigned int m_ID;
		Shared<VertexBuffer> m_VertexBuffer;
		Shared<IndexBuffer> m_IndexBuffer;

	};

}