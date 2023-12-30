#pragma once

#include "Core.h"
#include "glad.h"

#include <vector>

#include "Mesh.h"

namespace Cyclope {

	enum class CYCLOPE_API ShaderDataType
	{
		Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}

		return 0;
	}

	struct CYCLOPE_API BufferElement {
		ShaderDataType type;
		int offset = 0;
		int size;
		bool normalized;

		BufferElement() {};
		BufferElement(ShaderDataType _type, bool _normalized = false) 
		: type(_type), normalized(_normalized), size(ShaderDataTypeSize(type)){}

		unsigned int GetComponentCount() const
		{
			switch (type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3; 
			case ShaderDataType::Mat4:    return 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			return 0;
		}

	};

	class CYCLOPE_API BufferLayout
	{
	public:
		BufferLayout(){}
		BufferLayout(std::vector<BufferElement> elements)
			: m_elements(elements) {
			CalculateOffsetsAndStride();
		}

		int GetStride() const { return m_stride; }
		int GetOffset() const { return m_offset; }

		std::vector<BufferElement>& GetElements() { return m_elements; }

		static BufferLayout Standard() { return BufferLayout({ {ShaderDataType::Float3}, {ShaderDataType::Float2 } }); }//May change later

	private:
		void CalculateOffsetsAndStride();

		int m_stride = 0;
		int m_offset = 0;
		std::vector<BufferElement> m_elements;
	};

	class CYCLOPE_API VertexBuffer {

	public:
		virtual ~VertexBuffer() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetBufferLayout(const BufferLayout& layout) = 0;
		virtual void LinkVertexAttributes() = 0;

		virtual GLsizeiptr GetVertexCount() const = 0;

		static Shared<VertexBuffer> Create(float* vertices, GLsizeiptr size, const BufferLayout& layout);

	};

	class CYCLOPE_API IndexBuffer {

	public:
		virtual ~IndexBuffer() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual GLsizeiptr GetSize() const = 0;

		static Shared<IndexBuffer> Create(unsigned int* indices, GLsizeiptr size);

	};

}