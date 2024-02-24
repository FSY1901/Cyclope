#include "Cpch.h"
#include "OpenGLFramebuffer.h"

#include "glad.h"

namespace Cyclope {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specs) : m_specification(specs) {
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

		if (m_specification.samples == 1) {
			glGenTextures(1, &m_colorAttachment);
			glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specification.width, m_specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

			glGenTextures(1, &m_depthAttachment);
			glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);
		}
		else {
			glGenTextures(1, &m_colorAttachment);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorAttachment);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_specification.samples, GL_RGBA8, m_specification.width, m_specification.height, GL_TRUE);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_colorAttachment, 0);

			glGenTextures(1, &m_depthAttachment);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_depthAttachment);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_specification.samples, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height, GL_TRUE);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_depthAttachment, 0);
		}

#ifdef CYCLOPE_DEBUG
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			CYCLOPE_CORE_ERROR("Error when trying to create Framebuffer!");
		}
#endif // CYCLOPE_DEBUG


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::BlitTo(Shared<Framebuffer>& target){
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_ID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target->GetID());
		glBlitFramebuffer(0, 0, m_specification.width, m_specification.height, 0, 0, target->GetSpecification().width, target->GetSpecification().height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	}

	void OpenGLFramebuffer::Delete() {
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(1, &m_colorAttachment);
		glDeleteTextures(1, &m_depthAttachment);
	}

}