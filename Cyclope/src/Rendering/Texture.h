#include "Core.h"

#include "glad.h"

#ifndef TEXTURE_H
#define TEXTURE_H

namespace Cyclope {

	class CYCLOPE_API Texture2D {

	public:
		Texture2D();

		void Create(const char* filename, bool flipped = true);
		void Bind(int offset = 0);
		void Unbind();

		GLuint GetTexture() const;

		int GetWidth() const;
		int GetHeight() const;

	private:
		const char* m_filename;
		GLuint m_texture;

		int m_width, m_height;

	};

}

#endif 