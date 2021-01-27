#pragma once

#include "renderer_base.h"
#include "texture/texture.h"

#include <glm/glm.hpp>
#include <stb_image.h>


namespace Bubble
{
	struct Texture2DSpecification
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t ChanelFormat = GL_UNSIGNED_BYTE; // GL_UNSIGNED_BYTE, GL_FLOAT
		uint32_t InternalFormat = GL_RGBA8;       // GL_RED8, GL_RGB8, GL_RGBA8, GL_DEPTH_COMPONENT
		uint32_t DataFormat = GL_RGBA;            // GL_RED, GL_RGB, GL_RGBA , GL_DEPTH_COMPONENT
		uint32_t MinFiler = GL_LINEAR;            // GL_LINEAR, GL_NEAREST
		uint32_t MagFilter = GL_LINEAR;           // GL_LINEAR, GL_NEAREST
		uint32_t WrapS = GL_MIRRORED_REPEAT;	  // GL_REPEAT, GL_CLAMP, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT 
		uint32_t WrapT = GL_MIRRORED_REPEAT;	  // GL_REPEAT, GL_CLAMP, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT 
		glm::vec4 BorderColor = glm::vec4(1.0f);
		bool Flip = false;
		bool MinMap = false;
		bool AnisotropicFiltering = false;

		auto operator<=>(const Texture2DSpecification&) const = default;
	};

	struct Texture2D
	{
		uint32_t mRendererID = 0;
		Texture2DSpecification mSpecification;

	public:
		Texture2D() = default;
		// Create 1x1 texture
		Texture2D(const glm::vec4& color);
		Texture2D(uint32_t width, uint32_t height);
		Texture2D(const Texture2DSpecification& spec);
		Texture2D(const std::string& path, const Texture2DSpecification& spec = {});

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;

		Texture2D(Texture2D&&) noexcept;
		Texture2D& operator= (Texture2D&&) noexcept;

		~Texture2D();

		uint32_t GetWidth() const { return mSpecification.Width; }
		uint32_t GetHeight() const { return mSpecification.Height; }
		uint32_t GetRendererID() const  { return mRendererID; }

		// Size in bytes
		void SetData(void* data, uint32_t size);
		void GetData(void* data, uint32_t size) const;

		void Bind(uint32_t slot = 0) const;
		static void UnbindAll() { glActiveTexture(GL_TEXTURE0); }

		void Resize(const glm::ivec2& new_size);
		void Invalidate();

		bool operator==(const Texture2D& other) const { return mRendererID == other.mRendererID; }
		static std::tuple<Scope<uint8_t>, Texture2DSpecification> 
			OpenRawImage(const std::string& path, Texture2DSpecification spec = {});
	};


	void SetTextureSpecChanels(Texture2DSpecification& spec, int channels);
	uint32_t ExtractTextureSpecChannels(const Texture2DSpecification& spec);
	uint32_t GetTextureSize(const Texture2DSpecification& spec);
}