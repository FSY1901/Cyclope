#pragma once

#include "Core.h"

namespace Cyclope {

	enum class FramebufferTextureFormat {
		None = 0,
		RGBA8,
		RED_INTEGER,
		DEPTH24STENCIL8,
		DEPTH = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format) 
		: textureFormat(format) {}
		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
		: attachments(attachments) {}
		std::vector<FramebufferTextureSpecification> attachments;
	};

	struct FramebufferSpecification {
		uint32_t width, height;
		uint32_t samples = 1;
		FramebufferAttachmentSpecification attachments;
	};

	//API might change so that the attachments can be determined by user
	class CYCLOPE_API Framebuffer {
	public:
		virtual void Bind() const = 0;
		virtual void BindTexture(uint32_t id) const = 0;
		virtual void Unbind() const = 0;

		virtual void Invalidate() = 0;
		virtual void BlitTo(Shared<Framebuffer>& target) = 0;
		virtual void BlitTextureTo(Shared<Framebuffer>& target, uint32_t attachmentIndex) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
		virtual FramebufferSpecification& GetSpecification() = 0;

		virtual uint32_t GetID() = 0;
		virtual uint32_t GetColorAttachment(int index) = 0;
		virtual uint32_t GetDepthAttachment() = 0;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		static Shared<Framebuffer> Create(const FramebufferSpecification& specs);
	};

}