
#include "framebuffer.h"

namespace Bubble
{
	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
		: mSpecification(spec)
	{
		Invalidate();
	}

	Framebuffer::Framebuffer(int width, int height)
		: mSpecification({ {width, height} })
	{
		Invalidate();
	}

	Framebuffer::Framebuffer(Framebuffer&& other) noexcept
	{
		// Rebind
		mRendererID = other.mRendererID;
		mColorAttachment = other.mColorAttachment;
		mDepthAttachment = other.mDepthAttachment;
		mSpecification = other.mSpecification;

		// Make invalid
		other.mRendererID = 0;
		other.mColorAttachment = 0;
		other.mDepthAttachment = 0;
	}

	Framebuffer& Framebuffer::operator= (Framebuffer&& other) noexcept
	{
		if (this != &other)
		{
			// Clear
			glDeleteFramebuffers(1, &mRendererID);
			glDeleteTextures(1, &mColorAttachment);
			glDeleteTextures(1, &mDepthAttachment);

			// Rebind
			mRendererID = other.mRendererID;
			mColorAttachment = other.mColorAttachment;
			mDepthAttachment = other.mDepthAttachment;
			mSpecification = other.mSpecification;

			// Make invalid
			other.mRendererID = 0;
			other.mColorAttachment = 0;
			other.mDepthAttachment = 0;
		}
		return *this;
	}


	void Framebuffer::Create(const FramebufferSpecification& spec)
	{
		mSpecification = spec;
		Invalidate();
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &mRendererID);
		glDeleteTextures(1, &mColorAttachment);
		glDeleteTextures(1, &mDepthAttachment);
	}

	void Framebuffer::Invalidate()
	{
		glDeleteFramebuffers(1, &mRendererID);
		glDeleteTextures(1, &mColorAttachment);
		glDeleteTextures(1, &mDepthAttachment);

		glcall(glGenFramebuffers(1, &mRendererID));
		glcall(glBindFramebuffer(GL_FRAMEBUFFER, mRendererID));

		// Color
		glcall(glGenTextures(1, &mColorAttachment));
		glcall(glBindTexture(GL_TEXTURE_2D, mColorAttachment));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			GetWidth(), GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);

		// Depth
		glcall(glGenTextures(1, &mDepthAttachment));
		glcall(glBindTexture(GL_TEXTURE_2D, mDepthAttachment));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			GetWidth(), GetHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		
		float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachment, 0);

		// Something going wrong
		BUBBLE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int Framebuffer::GetWidth() const
	{
		return mSpecification.Size.x;
	}

	int Framebuffer::GetHeight() const
	{
		return mSpecification.Size.y;
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
		return mSpecification.Size;
	}

	void Framebuffer::Resize(glm::ivec2 size)
	{
		if ((size.x * size.y) <= 0) {
			return;
		}
		else if (size.x < size.y) {
			size.x = size.y;
		}
		mSpecification.Size = size;
		Invalidate();
	}

	const FramebufferSpecification& Framebuffer::GetSpecification() const
	{
		return mSpecification;
	}

	uint32_t Framebuffer::GetColorAttachmentRendererID()
	{
		return mColorAttachment;
	}

}