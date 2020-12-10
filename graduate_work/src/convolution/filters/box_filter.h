#pragma once

#include "kernel.h"


inline Kernel CreateBoxFilter(int size)
{
	Kernel kernel(size);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			kernel[i][j] = 1.0f / (size * size);
		}
	}
	return std::move(kernel);
}