#pragma once

#include "bubble.h"
#include "matricies.h"

struct Kernel
{
	static Scope<UniformBuffer> sKernelUBO;
	static Ref<Shader> sConvolutionShader;
	gm::mat mKernel;

	Kernel() = default;
	Kernel(int size);
	Kernel(std::initializer_list<gm::Vec<float>> values);

	int GetWidth() const { return mKernel.GetColsNum(); }
	int GetHeight() const { return mKernel.GetRowsNum(); }
	
	void Bind() const;
	Texture2D Apply (const Texture2D& src, Framebuffer&& fb = {}) const;
	float* operator[](int index) { return mKernel[index]; }
	const float* operator[] (int index) const { return mKernel[index]; }
	static void Init();
};