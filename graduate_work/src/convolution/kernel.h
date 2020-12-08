#pragma once

#include "bubble.h"
#include "matricies.h"

struct Kernel
{
	static Scope<UniformBuffer> sKernelUBO;
	gm::mat mKernel;

	Kernel() = default;
	Kernel(std::initializer_list<gm::Vec<float>> values)
		: mKernel{ std::move(values) }
	{}

	void Bind()
	{
		if (!sKernelUBO)
		{
			Kernel::Init();
		}
		sKernelUBO->SetData(mKernel.GetData(), mKernel.GetColsNum() * mKernel.GetRowsNum() * sizeof(float), 16);
		int size[2] = { mKernel.GetColsNum(),  mKernel.GetRowsNum() };
		sKernelUBO->SetData(size, sizeof(int) * 2);
	}

	static void Init()
	{
		sKernelUBO = CreateScope<UniformBuffer>(2, sizeof(glm::vec4) * 30);
	}
};