#ifndef VAO_H
#define VAO_H

#include "Core.h"
#include "glad.h"
#include "VBO.h"

namespace Cyclope {

#define TRIANGLES GL_TRIANGLES

	class API VAO {

	public:
		VAO();
		void Generate();
		void Link(VBO* VBO);
		void Bind();
		void Unbind();
		void Delete();
		void Draw(int mode, int first, int count);

	private:
		unsigned int m_ID;

	};

}

#endif