#include "Core.h"
#include "glad.h"

#ifndef EBO_H
#define EBO_H

namespace Cyclope {

	class CYCLOPE_API EBO {

	public:
		EBO();
		void SetData(unsigned int* indices, GLsizeiptr size);

		unsigned int getID();

		void Generate();
		void Bind();
		void Unbind();
		void Delete();

	private:
		unsigned int m_ID;

	};

}

#endif // !EBO_H