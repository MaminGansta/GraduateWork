#pragma once

#include <cstdint>


struct Pixel
{
	float x = 0, y = 0;
	float color[4] = {0};
	uint8_t channels = 0;

	Pixel& operator+= (const Pixel& other)
	{
		x += other.x;
		y += other.y;
		for (int channel = 0; channel < other.channels; channel++)
		{
			color[channel] += other.color[channel];
		}
		return *this;
	}

	Pixel& operator/= (float value)
	{
		x /= value;
		y /= value;
		for (int channel = 0; channel < 4; channel++)
		{
			color[channel] /= value;
		}
		return *this;
	}
};