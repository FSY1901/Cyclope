#ifndef VAO_H
#define VAO_H

#include "Core.h"
#include "glad.h"

#include "VBO.h"
#include "EBO.h"

namespace Cyclope {

#define TRIANGLES GL_TRIANGLES

	class CYCLOPE_API VAO {

	public:
		VAO();
		void Generate();
		void LinkVBO(VBO* vbo);
		void LinkEBO(VBO* vbo, EBO* ebo);
		void Bind();
		void Unbind();
		void Delete();
		void DrawVBO(int mode, int first, int count);
		void DrawEBO(int mode, int count);

	private:
		unsigned int m_ID;

	};

}

#endif