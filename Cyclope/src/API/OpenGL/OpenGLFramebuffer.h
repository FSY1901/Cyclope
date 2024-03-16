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
		void BlitTextureTo(Shared<Framebuffer>& target, uint32_t attachmentIndex) override;

		void ClearAttachment(uint32_t attachmentIndex, int value) override;

		const FramebufferSpecification& GetSpecification() const override { return m_specification; }
		FramebufferSpecification& GetSpecification() override { return m_specification; }

		uint32_t GetID() override { return m_ID; }
		uint32_t GetColorAttachment(int index) override { return m_colorAttachments[index]; };
		uint32_t GetDepthAttachment() override { return m_depthAttachment; };

		int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
	private:
		FramebufferSpecification m_specification;
		uint32_t m_ID = 0;

		std::vector<uint32_t> m_colorAttachments;
		uint32_t m_depthAttachment = 0;

		std::vector<FramebufferTextureSpecification> m_colorSpecs;
		FramebufferTextureSpecification m_depthSpecs = FramebufferTextureFormat::None;

		void Delete();

	};

}