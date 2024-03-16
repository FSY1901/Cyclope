#include "Cpch.h"
#include "OpenGLFramebuffer.h"

#include "glad.h"

namespace Cyclope {


	static GLenum TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(uint32_t* outID, uint32_t count)
	{
		glGenTextures(count, outID);
	}

	static void _BindTexture(bool multisampled, uint32_t id)
	{
		glBindTexture(TextureTarget(multisampled), id);
	}

	static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
	}

	static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
	}

	static bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
		}

		return false;
	}

	static GLenum FBTextureFormatToGL(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
		case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
		}

		return 0;
	}


	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specs) : m_specification(specs) {
		for (auto spec : specs.attachments.attachments) {
			if (!IsDepthFormat(spec.textureFormat))
				m_colorSpecs.emplace_back(spec);
			else
				m_depthSpecs = spec;
		}
		
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		Delete();
	}

	void OpenGLFramebuffer::Bind() const{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	}

	void OpenGLFramebuffer::BindTexture(uint32_t id) const
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void OpenGLFramebuffer::Unbind() const{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Invalidate() {

		if (m_ID != 0) {
			Delete();
		}

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		bool multisample = m_specification.samples > 1;
		if (m_colorSpecs.size()) {
			m_colorAttachments.resize(m_colorSpecs.size());
			CreateTextures(m_colorAttachments.data(), m_colorAttachments.size());
			for (int i = 0; i < m_colorSpecs.size(); i++) {
				switch (m_colorSpecs[i].textureFormat) {
				case FramebufferTextureFormat::RGBA8:
					_BindTexture(multisample, m_colorAttachments[i]);
					AttachColorTexture(m_colorAttachments[i], m_specification.samples, GL_RGBA8, GL_RGBA, m_specification.width,
						m_specification.height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					_BindTexture(multisample, m_colorAttachments[i]);
					AttachColorTexture(m_colorAttachments[i], m_specification.samples, GL_R32I, GL_RED_INTEGER, m_specification.width,
						m_specification.height, i);
					break;
				}
			}
		}

		if (m_depthSpecs.textureFormat != FramebufferTextureFormat::None)
		{
			CreateTextures(&m_depthAttachment, 1);
			_BindTexture(multisample, m_depthAttachment);
			switch (m_depthSpecs.textureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(m_depthAttachment, m_specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_specification.width, m_specification.height);
				break;
			}
		}

		if (m_colorAttachments.size() > 1)
		{
			//GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			//glDrawBuffers(m_colorAttachments.size(), buffers);
			GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
			glDrawBuffers(2, buffers);
		}
		else if (m_colorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		/*
		* m_colorAttachments.resize(2);
		if (m_specification.samples == 1) {
			glGenTextures(1, &m_colorAttachments[0]);
			glBindTexture(GL_TEXTURE_2D, m_colorAttachments[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specification.width, m_specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachments[0], 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glGenTextures(1, &m_colorAttachments[1]);
			glBindTexture(GL_TEXTURE_2D, m_colorAttachments[1]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specification.width, m_specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_colorAttachments[1], 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glGenTextures(1, &m_depthAttachment);
			glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {
			glGenTextures(1, &m_colorAttachments[0]);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorAttachments[0]);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_specification.samples, GL_RGBA8, m_specification.width, m_specification.height, GL_TRUE);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_colorAttachments[0], 0);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

			glGenTextures(1, &m_colorAttachments[1]);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorAttachments[1]);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_specification.samples, GL_RGBA8, m_specification.width, m_specification.height, GL_TRUE);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, m_colorAttachments[1], 0);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

			glGenTextures(1, &m_depthAttachment);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_depthAttachment);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_specification.samples, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height, GL_TRUE);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_depthAttachment, 0);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		}

		GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, buffers);
		*/

#ifdef CYCLOPE_DEBUG
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			CYCLOPE_CORE_ERROR("Error when trying to create Framebuffer!");
		}
#endif // CYCLOPE_DEBUG

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			CYCLOPE_TRACE("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	}

	void OpenGLFramebuffer::BlitTo(Shared<Framebuffer>& target){
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_ID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target->GetID());
		for (int i = 0; i < m_colorAttachments.size(); i++) {
			glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
			glBlitFramebuffer(0, 0, m_specification.width, m_specification.height, 0, 0, target->GetSpecification().width, target->GetSpecification().height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		}
	}

	void OpenGLFramebuffer::BlitTextureTo(Shared<Framebuffer>& target, uint32_t attachmentIndex) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_ID);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target->GetID());
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		glBlitFramebuffer(0, 0, m_specification.width, m_specification.height, 0, 0, target->GetSpecification().width, target->GetSpecification().height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
		auto& spec = m_colorSpecs[attachmentIndex];

		glClearTexImage(m_colorAttachments[attachmentIndex], 0, FBTextureFormatToGL(spec.textureFormat), GL_INT, &value);
	}

	void OpenGLFramebuffer::Delete() {
		glDeleteFramebuffers(1, &m_ID);
		//glDeleteTextures(1, &m_colorAttachment);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);
		m_colorAttachments.clear();
		m_depthAttachment = 0;
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
		int data;
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &data);
		return data;
	}

}