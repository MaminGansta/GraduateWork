
#include "conv_calculation_gpu.h"


namespace gpu
{
	Scope<UniformBuffer> sKernelUBO;
	Ref<Shader> sConvolutionShader;

	static void Init()
	{
		sKernelUBO = CreateScope<UniformBuffer>(0, sizeof(float) * 21 * 21 + 16); // Max kernel size is 21x21
		sConvolutionShader = ShaderLoader::Load("resources/shaders/convolution.glsl");
	}

	static void BindKernel(const Kernel& kernel)
	{
		if (!sKernelUBO)
		{
			Init();
		}
		int size[2] = { kernel.GetSize(),  kernel.GetSize() };
		sKernelUBO->SetData(size, sizeof(int) * 2);
		sKernelUBO->SetData(kernel.GetData(), pow(kernel.GetSize(), 2) * sizeof(float), 16);
	}

	Texture2D ApplyKernel(const Kernel& kernel, const Texture2D& image, Framebuffer&& fb)
	{
		if (image.GetWidth() != fb.GetWidth() ||
			image.GetHeight() != fb.GetHeight())
		{
			fb.Resize(image.GetWidth(), image.GetHeight());
		}
		fb.Bind();
		BindKernel(kernel);
		sConvolutionShader->SetTexture2D("uImage", image);
		Renderer::DrawIndices(Renderer::sFullScreenVAO);
		return std::move(fb.GetColorAttachment());
	}
}