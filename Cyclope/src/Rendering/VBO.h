#include "Core.h"
#include "glad.h"

#ifndef VBO_H
#define VBO_H

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