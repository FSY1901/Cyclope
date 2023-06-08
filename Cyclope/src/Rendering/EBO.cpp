#include "EBO.h"

namespace Cyclope {

	EBO::EBO() {}

	void EBO::Generate() {
		glGenBuffers(1, &m_ID);
	}

	void EBO::Bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void EBO::SetData(unsigned int* indices, GLsizeiptr size) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	void EBO::Unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void EBO::Delete() {
		glDeleteBuffers(1, &m_ID);
	}

	unsigned int EBO::getID() { return m_ID; }

}