#pragma once

#include "kernel.h"

inline Kernel CreateSharpnessFilter(int size, int sharpness = 10)
{
	Kernel kernel(size);
	float total = 0.0f;
	int pad = size / 2;

	float A, a;
	A = 1.0f / (2.0f * PI * (size * size));
	a = 1.0f / (2.0f * (size * size));

	for (int y = -pad; y <= pad; y++)
	{
		for (int x = -pad; x <= pad; x++)
		{
			if (x == 0 && y == 0) {
				continue;
			}
			float gauss = A * expf(-(a * (x * x) + 2 * (abs(x * y)) + a * (y * y))) * sharpness;
			kernel[y + pad][x + pad] = -gauss;
			total += -gauss;
		}
	}

	kernel[pad][pad] = -total + 1;
	return std::move(kernel);
}