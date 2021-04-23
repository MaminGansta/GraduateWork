#pragma once

#include "bubble.h"

#include <cstdint>
#include <string>


namespace cpu
{
	struct Image
	{
		Scope<uint8_t> mData;
		Texture2DSpecification mSpecification;

		Image() = default;
		Image(const std::string& path);
		Image(const Texture2DSpecification& spec);
		Image(const Texture2D& texture);

		uint8_t* GetData()     const { return mData.get(); }
		uint32_t GetWidth()    const { return mSpecification.Width; };
		uint32_t GetHeight()   const { return mSpecification.Height; };
		uint32_t GetChannels() const { return ExtractTextureSpecChannels(mSpecification); }

        uint8_t* GetColor(uint32_t i);
		uint8_t* GetColor(uint32_t x, uint32_t y);
        const uint8_t* GetColor(uint32_t i) const;
		const uint8_t* GetColor(uint32_t x, uint32_t y) const;
		void SetColor(const uint8_t* color, uint32_t x, uint32_t y);
		void SetColor(const uint8_t* color, uint32_t i);

		Texture2D LoadOnGPU() const;
		void LoadOnGPU(Texture2D& image) const;

		operator Texture2D() const { return LoadOnGPU(); }
	};

}