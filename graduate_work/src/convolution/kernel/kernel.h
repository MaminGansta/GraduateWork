#pragma once

#include "bubble.h"
#include "matricies.h"
#include "image/image.h"

struct Kernel
{
	static Scope<UniformBuffer> sKernelUBO;
	static Ref<Shader> sConvolutionShader;
	gm::mat mKernel;

	Kernel() = default;
	Kernel(int size);
	Kernel(std::initializer_list<gm::Vec<float>> values);

	int GetSize() const { return mKernel.GetColsNum(); }
	
	void Bind() const;
	Image Apply(const Image& src) const;
	Texture2D ApplyGPU (const Texture2D& src, Framebuffer&& fb = {}) const;
	
	float* operator[](int index) { return mKernel[index]; }
	const float* operator[] (int index) const { return mKernel[index]; }
	static void Init();
};