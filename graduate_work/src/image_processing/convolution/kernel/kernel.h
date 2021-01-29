#pragma once

#include "bubble.h"
#include "geometry.h"

struct Kernel
{
	gm::mat mKernel;

	Kernel() = default;
	Kernel(int size);
	Kernel(std::initializer_list<gm::Vec<float>> values);

	float* GetData() { return mKernel.GetData(); }
	const float* GetData() const { return mKernel.GetData(); }
	int GetSize() const { return mKernel.GetColsNum(); }

	float* operator[](int index) { return mKernel[index]; }
	const float* operator[] (int index) const { return mKernel[index]; }
};