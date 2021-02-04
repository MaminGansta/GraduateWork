#pragma once

#include "bubble.h"
#include "kernel.h"
#include "cpu/image_cpu.h"


namespace cpu
{
	Image ApplyKernel(const Kernel& kernel, const Image& src);
}