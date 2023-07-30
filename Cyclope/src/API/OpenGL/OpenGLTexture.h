#pragma once

#include "Core.h"
#include "../Rendering/Texture.h"

namespace Cyclope {

	class CYCLOPE_API OpenGLTexture2D : public Texture2D {

	public:
		OpenGLTexture2D(const char* m_path, bool flipped = true);
		OpenGLTexture2D(TextureSpecification spec, void* data);
		~OpenGLTexture2D() override;

		void Bind(unsigned int offset = 0) const override;
		void Unbind() const override;

		void SetData(void* data) const override;

		GLuint GetTexture() const override;
		int GetWidth() const override;
		int GetHeight() const override;

		const std::string& GetPath() const override;

	private:
		std::string m_path;
		GLuint m_texture;
		TextureSpecification m_spec;

	};

}