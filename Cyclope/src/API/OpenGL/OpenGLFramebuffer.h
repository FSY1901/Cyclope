#pragma once

#include "../Rendering/Framebuffer.h"

namespace Cyclope {

	class OpenGLFramebuffer : public Framebuffer {
	
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specs);
		virtual ~OpenGLFramebuffer();

		void Bind() const override;
		void BindTexture(uint32_t id) const override;
		void Unbind() const override;
	
		void Invalidate() override;
		void BlitTo(Shared<Framebuffer>& target) override;

		const FramebufferSpecification& GetSpecification() const override { return m_specification; }
		FramebufferSpecification& GetSpecification() override { return m_specification; }

		uint32_t GetID() override { return m_ID; }
		uint32_t GetColorAttachment() override { return m_colorAttachment; };
		uint32_t GetDepthAttachment() override { return m_depthAttachment; };
	private:
		FramebufferSpecification m_specification;
		uint32_t m_ID;

		uint32_t m_colorAttachment;
		uint32_t m_depthAttachment;

		void Delete();

	};

}