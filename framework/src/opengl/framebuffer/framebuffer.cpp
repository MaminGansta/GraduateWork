
#include "framebuffer.h"
#include "renderer/renderer.h"


namespace Bubble
{
	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
		: mSpecification(spec)
	{
		SetDefaultAttachemtSpec();
		Invalidate();
	}

	Framebuffer::Framebuffer(uint32_t width, uint32_t height)
		: mSpecification({width, height})
	{
		SetDefaultAttachemtSpec();
		Invalidate();
	}

	Framebuffer::Framebuffer(Texture2D&& color, Texture2D&& depth)
		: mColorAttachment(std::move(color)),
		  mDepthAttachment(std::move(depth))
	{
		mSpecification = { mColorAttachment.GetWidth(), mColorAttachment.GetHeight() };
		Invalidate();
	}

	Framebuffer::Framebuffer(Framebuffer&& other) noexcept
	{
		mRendererID = other.mRendererID;
		mColorAttachment = std::move(other.mColorAttachment);
		mDepthAttachment = std::move(other.mDepthAttachment);
		mSpecification = other.mSpecification;
		other.mRendererID = 0;
	}

	Framebuffer& Framebuffer::operator= (Framebuffer&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteFramebuffers(1, &mRendererID);
			mRendererID = other.mRendererID;
			mColorAttachment = std::move(other.mColorAttachment);
			mDepthAttachment = std::move(other.mDepthAttachment);
			mSpecification = other.mSpecification;
			other.mRendererID = 0;
		}
		return *this;
	}


	void Framebuffer::Create(const FramebufferSpecification& spec)
	{
		mSpecification = spec;
		SetDefaultAttachemtSpec();
		Invalidate();
	}

	void Framebuffer::SetColorAttachment(Texture2D&& texture)
	{
		mColorAttachment = std::move(texture);
	}

	void Framebuffer::SetDepthAttachment(Texture2D&& texture)
	{
		mDepthAttachment = std::move(texture);
	}

	Bubble::Texture2D Framebuffer::GetColorAttachment()
	{
		return std::move(mColorAttachment);
	}

	Bubble::Texture2D Framebuffer::GetDepthAttachment()
	{
		return std::move(mDepthAttachment);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &mRendererID);
	}

	void Framebuffer::Invalidate()
	{
		glDeleteFramebuffers(1, &mRendererID);
		glcall(glGenFramebuffers(1, &mRendererID));
		glcall(glBindFramebuffer(GL_FRAMEBUFFER, mRendererID));

		if (mColorAttachment.GetWidth() != mSpecification.Width ||
			mColorAttachment.GetHeight() != mSpecification.Height)
		{
			mColorAttachment.Resize({ mSpecification.Width, mSpecification.Height });
		}

		if (Renderer::sActiveMod == RenderingMod::Graphic3D &&
			(mDepthAttachment.GetWidth() != mSpecification.Width ||
			mDepthAttachment.GetHeight() != mSpecification.Height))
		{
			mDepthAttachment.Resize({ mSpecification.Width, mSpecification.Height });
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GetColorAttachmentRendererID(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GetDepthAttachmentRendererID(), 0);

		BUBBLE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int Framebuffer::GetWidth() const
	{
		return mSpecification.Width;
	}

	int Framebuffer::GetHeight() const
	{
		return mSpecification.Height;
	}

	void Framebuffer::Bind() const
	{
		glcall(glBindFramebuffer(GL_FRAMEBUFFER, mRendererID));
		glViewport(0, 0, GetWidth(), GetHeight());
	}

	void Framebuffer::Unbind() const
	{
		glcall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	const glm::ivec2& Framebuffer::GetSize() const
	{
		return { GetWidth(), GetHeight() };
	}

	void Framebuffer::Resize(glm::ivec2 size)
	{
		if (size.x < 0 || size.y < 0)
		{
			LOG_CORE_ERROR("Invalid framebuffer resize params width: {} height: {}", size.x, size.y);
			return;
		}
		mSpecification.Width = size.x;
		mSpecification.Height = size.y;
		Invalidate();
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width < 0 || height < 0)
		{
			LOG_CORE_ERROR("Invalid framebuffer resize params width: {} height: {}", width, height);
			return;
		}
		mSpecification.Width = width;
		mSpecification.Height = height;
		Invalidate();
	}

	const FramebufferSpecification& Framebuffer::GetSpecification() const
	{
		return mSpecification;
	}

	uint32_t Framebuffer::GetColorAttachmentRendererID()
	{
		return mColorAttachment.GetRendererID();
	}

	uint32_t Framebuffer::GetDepthAttachmentRendererID()
	{
		return mDepthAttachment.GetRendererID();

	}

	void Framebuffer::SetDefaultAttachemtSpec()
	{
		mDepthAttachment.mSpecification.ChanelFormat = GL_FLOAT;
		mDepthAttachment.mSpecification.DataFormat = GL_DEPTH_COMPONENT;
		mDepthAttachment.mSpecification.InternalFormat = GL_DEPTH_COMPONENT;
		mDepthAttachment.mSpecification.WrapS = GL_CLAMP_TO_BORDER;
		mDepthAttachment.mSpecification.WrapT = GL_CLAMP_TO_BORDER;
	}

}