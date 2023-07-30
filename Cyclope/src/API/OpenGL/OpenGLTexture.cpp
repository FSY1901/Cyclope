#include "OpenGLTexture.h"

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

	OpenGLTexture2D::OpenGLTexture2D(const char* m_path, bool flipped) {

        m_path = m_path;
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int nrChannels;
        stbi_set_flip_vertically_on_load(flipped);
        //for pngs
        m_spec.Format = ImageFormat::RGBA8;
        unsigned char* data = stbi_load(m_path, &m_spec.Width, &m_spec.Height, &nrChannels, STBI_rgb_alpha);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_spec.Width, m_spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture from " << std::string(m_path) << std::endl;
        }
        stbi_image_free(data);

	}

	OpenGLTexture2D::OpenGLTexture2D(TextureSpecification spec, void* data) {
        m_spec = spec;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
            GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, ImageFormatToGLInternalFormat(spec.Format), spec.Width,
            spec.Height, 0, ImageFormatToGLDataFormat(spec.Format), GL_UNSIGNED_BYTE,
            data);
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
    }

    GLuint OpenGLTexture2D::GetTexture() const { return m_texture; }

    int OpenGLTexture2D::GetWidth() const { return m_spec.Width; }
    int OpenGLTexture2D::GetHeight() const { return m_spec.Height; }

    const std::string& OpenGLTexture2D::GetPath() const { return m_path; }

}