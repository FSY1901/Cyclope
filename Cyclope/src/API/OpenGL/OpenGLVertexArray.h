#pragma once

#include "Core.h"

#include "../Rendering/VertexArray.h"

namespace Cyclope {


	class CYCLOPE_API OpenGLVertexArray : public VertexArray {

	public:
		OpenGLVertexArray(const Shared<VertexBuffer>& vertexBuffer, const Shared<IndexBuffer>& indexBuffer);
		~OpenGLVertexArray() override;

		void Bind() const override;
		void Unbind() const override;

		const Shared<IndexBuffer>& GetIndexBuffer() override;

	private:
		unsigned int m_ID;
		Shared<VertexBuffer> m_VertexBuffer;
		Shared<IndexBuffer> m_IndexBuffer;


	};

}