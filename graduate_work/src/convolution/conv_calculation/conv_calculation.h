#pragma once

#include "bubble.h"
#include "kernel.h"
#include "image/image.h"

namespace gpu
{
	Texture2D ApplyKernel(const Kernel& kernel, const Texture2D& image, Framebuffer&& fb);
}

namespace cpu
{
	Image ApplyKernel(const Kernel& kernel, const Image& src);
}