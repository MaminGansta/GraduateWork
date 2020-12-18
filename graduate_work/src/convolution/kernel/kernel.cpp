
#include "kernel.h"

Scope<UniformBuffer> Kernel::sKernelUBO;
Ref<Shader> Kernel::sConvolutionShader;

void Kernel::Init()
{
	sKernelUBO = CreateScope<UniformBuffer>(0, sizeof(float) * 21 * 21 + 16); // Max kernel size is 21x21
	sConvolutionShader = ShaderLoader::Load("resources/shaders/convolution.glsl");
}


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

Texture2D Kernel::ApplyGPU(const Texture2D& image, Framebuffer&& fb) const
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


Image Kernel::Apply(const Image& src) const
{
	Image res(src.mSpecification);
	
	int pad = GetSize() / 2;
	uint32_t channels = src.GetChannels();

	// Center area
	for (int y = pad; y < src.GetHeight() - pad; y++)
	{
		for (int x = pad; x < src.GetWidth() - pad; x++)
		{
			int32_t res_color[4] = { 0 };
			uint8_t clamped_color[4] = { 0 };

			for (int i = 0; i < GetSize(); i++)
			{
				for (int j = 0; j < GetSize(); j++)
				{
					const uint8_t* color = src.GetColor(j + x - pad, i + y - pad);

					for (int channel = 0; channel < channels; channel++)
					{
						res_color[channel] += color[channel] * mKernel[i][j];
					}
				}
			}

			for (int channel = 0; channel < channels; channel++)
			{
				clamped_color[channel] = std::clamp(res_color[channel], 0, 255);
			}

			res.SetColor(clamped_color, x, y);
		}
	}

	// Top, Bottom
	for (int i = 0; i < 2; i++)
	{
		for (int y = i * (src.GetHeight() - pad); y < (1 - i) * pad + i * src.GetHeight(); y++)
		{
			for (int x = 0; x < src.GetWidth(); x++)
			{
				int32_t res_color[4] = { 0 };
				uint8_t clamped_color[4] = { 0 };

				for (int i = 0; i < GetSize(); i++)
				{
					for (int j = 0; j < GetSize(); j++)
					{
						int core_y = abs(y - pad + i);
						int core_x = abs(x - pad + j);

						if (core_x >= src.GetWidth()) core_x = src.GetWidth() - (core_x - src.GetWidth()) - 1;
						if (core_y >= src.GetHeight()) core_y = src.GetHeight() - (core_y - src.GetHeight()) - 1;

						const uint8_t* color = src.GetColor(core_x, core_y);
						for (int channel = 0; channel < channels; channel++)
						{
							res_color[channel] += color[channel] * mKernel[i][j];
						}
					}
				}

				for (int channel = 0; channel < channels; channel++)
				{
					clamped_color[channel] = std::clamp(res_color[channel], 0, 255);
				}

				res.SetColor(clamped_color, x, y);
			}
		}
	}

	// Left, Right
	for (int i = 0; i < 2; i++)
	{
		for (int y = pad; y < src.GetHeight() - pad; y++)
		{
			for (int x = i * (src.GetWidth() - pad); x < (1 - i) * pad + i * src.GetWidth(); x++)
			{
				int32_t res_color[4] = { 0 };
				uint8_t clamped_color[4] = { 0 };

				for (int i = 0; i < GetSize(); i++)
				{
					for (int j = 0; j < GetSize(); j++)
					{
						int core_y = abs(y - pad + i);
						int core_x = abs(x - pad + j);

						if (core_x >= src.GetWidth()) core_x = src.GetWidth() - (core_x - src.GetWidth()) - 1;
						if (core_y >= src.GetHeight()) core_y = src.GetHeight() - (core_y - src.GetHeight()) - 1;

						const uint8_t* color = src.GetColor(core_x, core_y);
						for (int channel = 0; channel < channels; channel++)
						{
							res_color[channel] += color[channel] * mKernel[i][j];
						}
					}
				}

				for (int channel = 0; channel < channels; channel++)
				{
					clamped_color[channel] = std::clamp(res_color[channel], 0, 255);
				}

				res.SetColor(clamped_color, x, y);
			}
		}
	}

	return res;
}

