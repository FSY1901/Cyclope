#include "OpenGLTexture.h"
#include "Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

namespace Cyclope {

    static GLenum ImageFormatToGLDataFormat(ImageFormat format)
    {
        switch (format)
        {
            case ImageFormat::RGB8:     return GL_RGB;
            case ImageFormat::RGBA8:    return GL_RGBA;
        }

        return 0;
    }

    static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
    {
        switch (format)
        {
            case ImageFormat::RGB8:     return GL_RGB8;
            case ImageFormat::RGBA8:    return GL_RGBA8;
        }

        return 0;
    }

	OpenGLTexture2D::OpenGLTexture2D(TextureSpecification spec, void* data) {
        m_spec = spec;
        
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
            GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
        
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, ImageFormatToGLInternalFormat(spec.Format), spec.Width,
                spec.Height, 0, ImageFormatToGLDataFormat(spec.Format), GL_UNSIGNED_BYTE,
                data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            CYCLOPE_CORE_WARN("Data for texture was empty");
        }

        stbi_image_free(data);
	}

    OpenGLTexture2D::~OpenGLTexture2D() {
        glDeleteTextures(1, &m_texture);
    }

    void OpenGLTexture2D::Bind(unsigned int offset) const{
        glActiveTexture(GL_TEXTURE0 + offset); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }

    void OpenGLTexture2D::Unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture2D::SetData(void* data) const {
        glTexImage2D(GL_TEXTURE_2D, 0, ImageFormatToGLInternalFormat(m_spec.Format), m_spec.Width,
            m_spec.Height, 0, ImageFormatToGLDataFormat(m_spec.Format), GL_UNSIGNED_BYTE,
            data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    GLuint OpenGLTexture2D::GetTexture() const { return m_texture; }

    int OpenGLTexture2D::GetWidth() const { return m_spec.Width; }
    int OpenGLTexture2D::GetHeight() const { return m_spec.Height; }

}