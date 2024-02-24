#pragma once

#include "Core.h"

namespace Cyclope {

	struct FramebufferSpecification {
		uint32_t width, height;
		uint32_t samples = 1;
	};

	//API might change so that the attachments can be determined by user
	class CYCLOPE_API Framebuffer {
	public:
		virtual void Bind() const = 0;
		virtual void BindTexture(uint32_t id) const = 0;
		virtual void Unbind() const = 0;

		virtual void Invalidate() = 0;
		virtual void BlitTo(Shared<Framebuffer>& target) = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
		virtual FramebufferSpecification& GetSpecification() = 0;

		virtual uint32_t GetID() = 0;
		virtual uint32_t GetColorAttachment() = 0;
		virtual uint32_t GetDepthAttachment() = 0;

		static Shared<Framebuffer> Create(const FramebufferSpecification& specs);
	};

}