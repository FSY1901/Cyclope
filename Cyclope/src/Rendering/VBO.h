#ifndef VBO_H
#define VBO_H

#include "Core.h"
#include "glad.h"

namespace Cyclope {

	class CYCLOPE_API VBO {

	public:
		VBO();
		void SetData(float* vertices, GLsizeiptr size);
		unsigned int getID();

		void Generate();
		void Bind();
		void Unbind();
		void Delete();

	private:
		unsigned int m_ID;

	};

}

#endif 