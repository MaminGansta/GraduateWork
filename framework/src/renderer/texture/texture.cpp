
#include "texture.h"

#include "stb_image.h"


namespace Bubble
{
	Texture2D::Texture2D(const glm::vec4& color)
	{
		mSpecification.Width = 1;
		mSpecification.Height = 1;
		mSpecification.InternalFormat = GL_RGBA;
		mSpecification.DataFormat = GL_UNSIGNED_BYTE;
		mSpecification.MinFiler = GL_NEAREST;
		mSpecification.MagFilter = GL_NEAREST;
		mSpecification.WrapS = GL_REPEAT;
		mSpecification.WrapT = GL_REPEAT;
		Invalidate();
	}


	Texture2D::Texture2D(const Texture2DSpecification& spec)
		: mSpecification(spec)
	{
		Invalidate();
	}


	Texture2D::Texture2D(const std::string& path, const Texture2DSpecification& spec)
		: mSpecification(spec)
	{
		stbi_uc* data = nullptr;
		int width, height, channels;

		stbi_set_flip_vertically_on_load(spec.Flip);
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		
		if (data == nullptr)
			throw std::runtime_error("Failed to load image!\nPath: " + path);

		mSpecification.Width = width;
		mSpecification.Height = height;

		switch (channels)
		{
			case 1:
				mSpecification.InternalFormat = GL_R8;
				mSpecification.DataFormat = GL_RED;
				break;
			case 3:
				mSpecification.InternalFormat = GL_RGB8;
				mSpecification.DataFormat = GL_RGB;
				break;
			case 4:
				mSpecification.InternalFormat = GL_RGBA8;
				mSpecification.DataFormat = GL_RGBA;
				break;
			default:
				BUBBLE_CORE_ASSERT(false, "Format not supported!");
		}
		Invalidate();
		SetData(data, width * height * channels);
		stbi_image_free(data);
	}

	Texture2D::Texture2D(Texture2D&& other) noexcept
		: mRendererID(other.mRendererID),
		  mSpecification(other.mSpecification)
	{
		other.mSpecification.Width = 0;
		other.mSpecification.Height = 0;
		other.mRendererID = 0;
	}

	Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteTextures(1, &mRendererID);
			mRendererID = other.mRendererID;
			mSpecification = other.mSpecification;
			other.mRendererID = 0;
			other.mSpecification.Width = 0;
			other.mSpecification.Height = 0;
		}
		return *this;
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &mRendererID);
	}

	void Texture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = 0;
		switch (mSpecification.DataFormat)
		{
			case GL_RGBA:
				bpp = 4;
				break;
			case GL_RGB:
				bpp = 3;
				break;
			case GL_RED:
				bpp = 1;
				break;
			default:
				BUBBLE_CORE_ASSERT(false, "Format not supported!");
		}
		BUBBLE_CORE_ASSERT(size == mSpecification.Width * mSpecification.Height * bpp, "Data must be entire texture!");
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
			mSpecification.Width, mSpecification.Height, mSpecification.DataFormat, mSpecification.ChanelFormat, data);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, mRendererID);
	}

	std::tuple<Scope<uint8_t>, Texture2DSpecification>
		Texture2D::OpenRawImage(const std::string& path, Texture2DSpecification spec)
	{
		uint8_t* data = nullptr;
		int width, height, channels;

		stbi_set_flip_vertically_on_load(spec.Flip);
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		
		if (data == nullptr)
			throw std::runtime_error("Failed to load image!\nPath: " + path);
		
		spec.Width = width;
		spec.Height = height;

		switch (channels)
		{
			case 1:
				spec.InternalFormat = GL_R8;
				spec.DataFormat = GL_RED;
				break;
			case 3:
				spec.InternalFormat = GL_RGB8;
				spec.DataFormat = GL_RGB;
				break;
			case 4:
				spec.InternalFormat = GL_RGBA8;
				spec.DataFormat = GL_RGBA;
				break;
			default:
				BUBBLE_CORE_ASSERT(false, "Format not supported!");
		}
		return { Scope<uint8_t>(data), spec };
	}

	void Texture2D::Resize(const glm::ivec2& new_size)
	{
		mSpecification.Width = new_size.x;
		mSpecification.Height = new_size.y;
		Invalidate();
	}

	void Texture2D::Invalidate()
	{
		glDeleteTextures(1, &mRendererID);

		glcall(glGenTextures(1, &mRendererID));
		glcall(glBindTexture(GL_TEXTURE_2D, mRendererID));
		glcall(glTexImage2D(GL_TEXTURE_2D, 0, mSpecification.InternalFormat,
			mSpecification.Width, mSpecification.Height, 0, mSpecification.DataFormat, mSpecification.ChanelFormat, nullptr));

		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mSpecification.MinFiler));
		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mSpecification.MagFilter));

		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mSpecification.WrapS));
		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mSpecification.WrapT));

		if (mSpecification.WrapS == GL_CLAMP_TO_BORDER || mSpecification.WrapT == GL_CLAMP_TO_BORDER)
		{
			glcall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float*)&mSpecification.BorderColor));
		}

		if (mSpecification.AnisotropicFiltering)
		{
			GLfloat value;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);
		}

		if (mSpecification.MinMap && !mSpecification.AnisotropicFiltering)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

}