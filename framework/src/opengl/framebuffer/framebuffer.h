#pragma once

#include "renderer_base.h"
#include "texture/texture.h"

#include "glm/glm.hpp"
#include <cstdint>
#include <cassert>


namespace Bubble
{
	struct FramebufferSpecification
	{
		uint32_t Width;
		uint32_t Height;
		int Samples = 1;
	};

	struct Framebuffer
	{
		uint32_t mRendererID = 0;
		Texture2D mColorAttachment;
		Texture2D mDepthAttachment;
		FramebufferSpecification mSpecification;

		Framebuffer() = default;
		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator= (const Framebuffer&) = delete;

		Framebuffer(Framebuffer&& other) noexcept;
		Framebuffer& operator = (Framebuffer&& other) noexcept;

		Framebuffer(const FramebufferSpecification& spec);
		Framebuffer(uint32_t width, uint32_t height);
		Framebuffer(Texture2D&& color, Texture2D&& depth = Texture2D());

		void Create(const FramebufferSpecification& spec);
		void SetColorAttachment(Texture2D&& texture);
		void SetDepthAttachment(Texture2D&& texture);
		Texture2D GetColorAttachment();
		Texture2D GetDepthAttachment();

		virtual ~Framebuffer();

		void Bind() const;
		void Unbind() const;
		void Invalidate();

		int GetWidth() const;
		int GetHeight() const;
		const glm::ivec2& GetSize() const;
		void Resize(glm::ivec2 size);
		void Resize(uint32_t width, uint32_t height);

		uint32_t GetColorAttachmentRendererID();
		uint32_t GetDepthAttachmentRendererID();
		const FramebufferSpecification& GetSpecification() const;

	private:
		void SetDefaultAttachemtSpec();
	};
}