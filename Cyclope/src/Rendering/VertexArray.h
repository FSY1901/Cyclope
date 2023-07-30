#pragma once 

#include "Core.h"

#include "Buffer.h"

namespace Cyclope {

	class CYCLOPE_API VertexArray {

	public:
		virtual ~VertexArray() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const Shared<IndexBuffer>& GetIndexBuffer() = 0;

		static Shared<VertexArray> Create(const Shared<VertexBuffer>& vertexBuffer, const Shared<IndexBuffer>& indexBuffer);

	};

}