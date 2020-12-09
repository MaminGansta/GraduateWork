#pragma once

#include "bubble.h"
#include "matricies.h"

struct Kernel
{
	static Scope<UniformBuffer> sKernelUBO;
	static Ref<Shader> sConvolutionShader;
	gm::mat mKernel;

	Kernel() = default;
	Kernel(std::initializer_list<gm::Vec<float>> values);
	
	void Bind();
	Texture2D Apply(const Texture2D& src, Framebuffer&& fb = {});
	static void Init();
};