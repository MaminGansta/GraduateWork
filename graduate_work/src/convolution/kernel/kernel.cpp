
#include "kernel.h"


Kernel::Kernel(std::initializer_list<gm::Vec<float>> values)
	: mKernel{ std::move(values) }
{}

Kernel::Kernel(int size)
	: mKernel(size, size)
{
	BUBBLE_CORE_ASSERT(size % 2, "Invalid kernel size, it must be odd");
}
