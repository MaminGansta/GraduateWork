#pragma once

#include "renderer_base.h"
#include "texture/texture.h"
#include "glm/vec4.hpp"


namespace Bubble
{
	struct Texture2DSpecification
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t InternalFormat = GL_RGBA8;
		uint32_t DataFormat = GL_RGBA;
		uint32_t MinFiler = GL_LINEAR;
		uint32_t MagFilter = GL_LINEAR;
		uint32_t WrapS = GL_REPEAT;
		uint32_t WrapT = GL_REPEAT;
		glm::vec4 BorderColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		bool Flip = false;
		bool MinMap = true;
	};

	struct Texture2D
	{
		uint32_t mWidth = 0;
		uint32_t mHeight = 0;
		uint32_t mRendererID = 0;
		uint32_t mInternalFormat = 0, mDataFormat = 0;

	public:
		Texture2D() = default;
		// Create 1x1 texture
		Texture2D(const glm::vec4& color);
		Texture2D(const Texture2DSpecification& spec);
		Texture2D(const std::string& path, const Texture2DSpecification& spec = {});

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;

		Texture2D(Texture2D&&) noexcept;
		Texture2D& operator= (Texture2D&&) noexcept;

		~Texture2D();

		uint32_t GetWidth() const { return mWidth; }
		uint32_t GetHeight() const { return mHeight; }
		uint32_t GetRendererID() const  { return mRendererID; }

		void SetData(void* data, uint32_t size);

		void Bind(uint32_t slot = 0) const;
		static void UnbindAll() { glActiveTexture(GL_TEXTURE0); }

		bool operator==(const Texture2D& other) const {return mRendererID == other.mRendererID;}

		// Don't forget to remove char*
		static std::tuple<uint8_t*, Texture2DSpecification> OpenRawImage(const std::string& path);
	};
}