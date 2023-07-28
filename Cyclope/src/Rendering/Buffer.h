#pragma once

#include "Core.h"
#include "glad.h"

namespace Cyclope {

	class CYCLOPE_API VertexBuffer {

	public:
		virtual ~VertexBuffer() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shared<VertexBuffer> Create(float* vertices, GLsizeiptr size);

	};

	class CYCLOPE_API IndexBuffer {

	public:
		virtual ~IndexBuffer() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual GLsizeiptr GetSize() const = 0;

		static Shared<IndexBuffer> Create(unsigned int* indices, GLsizeiptr size);

	};

	class CYCLOPE_API VertexArray {

	public:
		virtual ~VertexArray() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const Shared<IndexBuffer>& GetIndexBuffer() = 0;

		static Shared<VertexArray> Create(const Shared<VertexBuffer>& vertexBuffer, const Shared<IndexBuffer>& indexBuffer);

	};

}