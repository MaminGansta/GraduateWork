#pragma once

#include "bubble.h"
#include "convolution/kernel.h"
#include "gpu/image_gpu.h"

namespace gpu
{
	Texture2D ApplyKernel(const Kernel& kernel, const Texture2D& image, Framebuffer&& fb);
}
