#pragma once

#include "kernel.h"

static const float PI = 3.1415926535897f;

// Best sigma is size * 6 + 1
inline Kernel CreateGaussFilter(int size)
{
	Kernel kernel(size);

	float A, a;
	A = 1.0f / (2.0f * PI * (size * size));
	a = 1.0f / (2.0f *  (size * size));

	float total = 0;
	int pad = size / 2;

	for (int y = -pad; y <= pad; y++)
	{
		for (int x = -pad; x <= pad; x++)
		{
			float gauss = A * expf(-(a * (x * x) + 2 * (abs(x * y)) + a * (y * y)));
			kernel[y + pad][x + pad] = gauss;
			total += gauss;
		}
	}
	
	total = 1.0f / total;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			kernel[i][j] *= total;
		}
	}

	return std::move(kernel);
}