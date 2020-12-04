
#include "texture.h"

#include "stb_image.h"


namespace Bubble
{
	Texture2D::Texture2D(const glm::vec4& color)
	{
		mWidth = 1;
		mHeight = 1;
		mInternalFormat = GL_RGBA;
		mDataFormat = GL_UNSIGNED_BYTE;

		glcall(glGenTextures(1, &mRendererID));
		glcall(glBindTexture(GL_TEXTURE_2D, mRendererID));
		glcall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color));

		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	}


	Texture2D::Texture2D(const Texture2DSpecification& spec)
		: mWidth(spec.Width),
		  mHeight(spec.Height)
	{
		mInternalFormat = spec.InternalFormat;
		mDataFormat = spec.DataFormat;

		glcall(glGenTextures(1, &mRendererID));
		glcall(glBindTexture(GL_TEXTURE_2D, mRendererID));
		glcall(glTexImage2D(GL_TEXTURE_2D, 0, spec.InternalFormat, spec.Width, spec.Height, 0, spec.DataFormat, GL_UNSIGNED_BYTE, nullptr));
		
		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, spec.MinFiler));
		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, spec.MagFilter));
		
		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, spec.WrapS));
		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, spec.WrapT));

		if (spec.WrapS == GL_CLAMP_TO_BORDER || spec.WrapT == GL_CLAMP_TO_BORDER)
		{
			glcall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float*)&spec.BorderColor));
		}

	}


	Texture2D::Texture2D(const std::string& path, const Texture2DSpecification& spec)
	{
		stbi_set_flip_vertically_on_load(spec.Flip);
		int width, height, channels;
		stbi_uc* data = nullptr;
		
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		
		if (data == nullptr)
			throw std::runtime_error("Failed to load image!\nPath: " + path);

		mWidth = width;
		mHeight = height;

		GLenum internal_format = 0;
		GLenum data_format = 0;
		if (channels == 1)
		{
			internal_format = GL_R8;
			data_format = GL_RED;
		}
		else if (channels == 3)
		{
			internal_format = GL_RGB8;
			data_format = GL_RGB;
		}
		else if (channels == 4)
		{
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
		}
		
		mInternalFormat = internal_format;
		mDataFormat = data_format;

		BUBBLE_CORE_ASSERT(internal_format & data_format, "Format not supported!");

		glcall(glGenTextures(1, &mRendererID));
		glcall(glBindTexture(GL_TEXTURE_2D, mRendererID));
		glcall(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data));
		
		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, spec.MinFiler));
		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, spec.MagFilter));

		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, spec.WrapS));
		glcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, spec.WrapT));

		if (spec.WrapS == GL_CLAMP_TO_BORDER || spec.WrapT == GL_CLAMP_TO_BORDER)
		{
			glcall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float*)&spec.BorderColor));
		}

		// Anisotropy filtering
		GLfloat value;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);

		//glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

	Texture2D::Texture2D(Texture2D&& other) noexcept
		: mWidth(other.mWidth),
		  mHeight(other.mHeight),
		  mDataFormat(other.mDataFormat),
		  mRendererID(other.mRendererID),
		  mInternalFormat(other.mInternalFormat)
	{
		other.mWidth = 0;
		other.mHeight = 0;
		other.mRendererID = 0;
	}

	Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteTextures(1, &mRendererID);
			mWidth = other.mWidth;
			mHeight = other.mHeight;
			mDataFormat = other.mDataFormat;
			mRendererID = other.mRendererID;
			mInternalFormat = other.mInternalFormat;
			other.mWidth = 0;
			other.mHeight = 0;
			other.mRendererID = 0;
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
		if (mDataFormat == GL_RGBA) {
			bpp = 4;
		}
		else if (mDataFormat == GL_RGB) {
			bpp = 3;
		}
		else if (mDataFormat == GL_RED) {
			bpp = 1;
		}

		BUBBLE_CORE_ASSERT(size == mWidth * mHeight * bpp, "Data must be entire texture!");
		glTexSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, mRendererID);
	}

	std::tuple<uint8_t*, Texture2DSpecification>
		Texture2D::OpenRawImage(const std::string& path)
	{
		//stbi_set_flip_vertically_on_load(spec.Flip);
		Texture2DSpecification spec;

		int width, height, channels;
		uint8_t* data = nullptr;

		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		
		if (data == nullptr) {
			throw std::runtime_error("Failed to load image!\nPath: " + path);
		}
		spec.Width = width;
		spec.Height = height;

		if (channels == 1)
		{
			spec.InternalFormat = GL_R8;
			spec.DataFormat = GL_RED;
		}
		else if (channels == 3)
		{
			spec.InternalFormat = GL_RGB8;
			spec.DataFormat = GL_RGB;
		}
		else if (channels == 4)
		{
			spec.InternalFormat = GL_RGBA8;
			spec.DataFormat = GL_RGBA;
		}
		else {
			BUBBLE_CORE_ASSERT(spec.InternalFormat & spec.DataFormat, "Format not supported!");
		}
		
		return { data, spec };
	}

}