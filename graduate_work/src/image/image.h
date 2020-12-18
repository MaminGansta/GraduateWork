#pragma once

#include "bubble.h"
#include <cstdint>
#include <string>

struct Image
{
	Scope<uint8_t> mData;
	Texture2DSpecification mSpecification;

	Image() = default;
	Image(const std::string& path);
	Image(const Texture2DSpecification& spec);

	uint8_t* GetData() const { return mData.get(); }
	uint32_t GetWidth() const { return mSpecification.Width; };
	uint32_t GetHeight() const { return mSpecification.Height; };
	uint32_t GetChannels() const { return ExtractTextureSpecChannels(mSpecification); }

	uint8_t* GetColor(uint32_t x, uint32_t y);
	const uint8_t* GetColor(uint32_t x, uint32_t y) const;
	void SetColor(const uint8_t* color, uint32_t x, uint32_t y);

	Texture2D LoadOnGPU();
};