#pragma once

#include "renderer_base.h"

#include "glm/glm.hpp"
#include <cstdint>
#include <cassert>


namespace Bubble
{
	struct FramebufferSpecification
	{
		glm::ivec2 Size;
		int Samples = 1;
		bool SwapChainTarget = false;
	};

	struct Framebuffer
	{
		uint32_t mRendererID = 0;
		uint32_t mColorAttachment = 0;
		uint32_t mDepthAttachment = 0;
		FramebufferSpecification mSpecification;

		Framebuffer() = default;
		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator= (const Framebuffer&) = delete;

		Framebuffer(Framebuffer&& other) noexcept;
		Framebuffer& operator = (Framebuffer&& other) noexcept;

		Framebuffer(const FramebufferSpecification& spec);
		Framebuffer(int width, int height);

		void Create(const FramebufferSpecification& spec);

		virtual ~Framebuffer();

		void Bind() const;
		void Unbind() const;
		void Invalidate();

		int GetWidth() const;
		int GetHeight() const;
		const glm::ivec2& GetSize() const;
		void Resize(glm::ivec2 size);

		uint32_t GetColorAttachmentRendererID();
		const FramebufferSpecification& GetSpecification() const;
	};
}