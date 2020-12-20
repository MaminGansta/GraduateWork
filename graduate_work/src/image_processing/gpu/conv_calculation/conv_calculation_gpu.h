#pragma once

#include "bubble.h"
#include "kernel.h"
#include "gpu/image/image_gpu.h"

namespace gpu
{
	Texture2D ApplyKernel(const Kernel& kernel, const Texture2D& image, Framebuffer&& fb);
}
