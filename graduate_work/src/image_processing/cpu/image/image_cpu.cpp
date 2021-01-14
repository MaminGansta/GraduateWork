
#include "image_cpu.h"


namespace cpu
{
	Image::Image(const std::string& path)
	{
		uint8_t* data;
		int width, height, channels;

		data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
			throw std::runtime_error("Failed to load image!\nPath: " + path);

		mData = Scope<uint8_t>((uint8_t*)malloc(width * height * 4));
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int raw_pos = (y * width + x) * channels;
				int res_pos = (y * width + x) * 4;
				memmove(mData.get() + res_pos, data + raw_pos, channels);
				mData.get()[res_pos + 3] = 255;
			}
		}
		mSpecification.Width = width;
		mSpecification.Height = height;
		SetTextureSpecChanels(mSpecification, 4);

		free(data);

		//auto [data, spec] = Texture2D::OpenRawImage(path);
		//mData = std::move(data);
		//mSpecification = spec;
	}


	Image::Image(const Texture2DSpecification& spec)
		: mSpecification(spec)
	{
		uint8_t* data = (uint8_t*)malloc(GetWidth() * GetHeight() * GetChannels());
		memset(data, 255, GetWidth() * GetHeight() * GetChannels());
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

	void Image::LoadOnGPU(Texture2D& image)
	{
		if (mSpecification != image.mSpecification)
		{
			image.mSpecification = mSpecification;
			image.Invalidate();
		}


	}

}