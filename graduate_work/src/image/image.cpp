
#include "image.h"


Image::Image(const std::string& path)
{
	auto [data, spec] = Texture2D::OpenRawImage(path);
	mData = std::move(data);
	mSpecification = spec;
}


Image::Image(const Texture2DSpecification& spec)
	: mSpecification(spec)
{
	uint8_t* data = (uint8_t*)malloc(GetWidth() * GetHeight() * GetChannels());
	mData = Scope<uint8_t>(data);
}

const uint8_t* Image::GetColor(uint32_t x, uint32_t y) const
{
	y *= GetChannels();
	x *= GetChannels();
	uint8_t* data = GetData();
	uint8_t* color = &data[y * GetWidth() + x];
	return color;
}

uint8_t* Image::GetColor(uint32_t x, uint32_t y)
{
	y *= GetChannels();
	x *= GetChannels();
	uint8_t* data = GetData();
	uint8_t* color = &data[y * GetWidth() + x];
	return color;
}

void Image::SetColor(const uint8_t* color, uint32_t x, uint32_t y)
{
	y *= GetChannels();
	x *= GetChannels();
	uint8_t* data = GetData();
	memmove(&data[y * GetWidth() + x], color, GetChannels());
}

Texture2D Image::LoadOnGPU()
{
	Texture2D texture(mSpecification);
	texture.SetData(mData.get(), GetWidth() * GetHeight() * GetChannels());
	return texture;
}