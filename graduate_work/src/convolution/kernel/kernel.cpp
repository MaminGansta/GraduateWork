
#include "kernel.h"

Scope<UniformBuffer> Kernel::sKernelUBO;
Ref<Shader> Kernel::sConvolutionShader;

Kernel::Kernel(std::initializer_list<gm::Vec<float>> values)
	: mKernel{ std::move(values) }
{}

Kernel::Kernel(int size)
	: mKernel(size, size)
{
	BUBBLE_CORE_ASSERT(size % 2, "Invalid kernel size, it must be odd");
}

void Kernel::Bind() const
{
	if (!sKernelUBO)
	{
		Kernel::Init();
	}
	sKernelUBO->SetData(mKernel.GetData(), mKernel.GetColsNum() * mKernel.GetRowsNum() * sizeof(float), 16);
	int size[2] = { mKernel.GetColsNum(),  mKernel.GetRowsNum() };
	sKernelUBO->SetData(size, sizeof(int) * 2);
}

Texture2D Kernel::Apply(const Texture2D& image, Framebuffer&& fb) const
{
	if (image.GetWidth() != fb.GetWidth() || image.GetHeight() != fb.GetHeight())
	{
		fb.Resize(image.GetWidth(), image.GetHeight());
	}
	Bind();
	fb.Bind();
	sConvolutionShader->SetTexture2D("uImage", image);
	Renderer::DrawIndices(Renderer::sFullScreenVAO);
	return std::move(fb.GetColorAttachment());
}

void Kernel::Init()
{
	sKernelUBO = CreateScope<UniformBuffer>(0, sizeof(float) * 21 * 21); // Max kernel size 21x21
	sConvolutionShader = ShaderLoader::Load("resources/shaders/convolution.glsl");
}

