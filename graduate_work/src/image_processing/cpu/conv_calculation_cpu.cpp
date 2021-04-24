
#include "conv_calculation_cpu.h"

namespace cpu
{
	Image ApplyKernel(const Kernel& kernel, const Image& src)
	{
		Image res(src.mSpecification);

		int pad = kernel.GetSize() / 2;
		uint32_t channels = src.GetChannels();

		// Center area
		#pragma omp parallel for
		for (int y = pad; y < src.GetHeight() - pad; y++)
		{
			for (int x = pad; x < src.GetWidth() - pad; x++)
			{
				int32_t res_color[4] = { 0 };
				uint8_t clamped_color[4] = { 0 };

				for (int i = 0; i < kernel.GetSize(); i++)
				{
					for (int j = 0; j < kernel.GetSize(); j++)
					{
						const uint8_t* color = src.GetColor(j + x - pad, i + y - pad);

						for (int channel = 0; channel < channels; channel++)
						{
							res_color[channel] += color[channel] * kernel[i][j];
						}
					}
				}

				for (int channel = 0; channel < channels; channel++)
				{
					clamped_color[channel] = std::clamp(res_color[channel], 0, 255);
				}

				if (channels == 4)
					clamped_color[3] = 255;

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

					for (int i = 0; i < kernel.GetSize(); i++)
					{
						for (int j = 0; j < kernel.GetSize(); j++)
						{
							int core_y = abs(y - pad + i);
							int core_x = abs(x - pad + j);

							if (core_x >= src.GetWidth()) core_x = src.GetWidth() - (core_x - src.GetWidth()) - 1;
							if (core_y >= src.GetHeight()) core_y = src.GetHeight() - (core_y - src.GetHeight()) - 1;

							const uint8_t* color = src.GetColor(core_x, core_y);
							for (int channel = 0; channel < channels; channel++)
							{
								res_color[channel] += color[channel] * kernel[i][j];
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

					for (int i = 0; i < kernel.GetSize(); i++)
					{
						for (int j = 0; j < kernel.GetSize(); j++)
						{
							int core_y = abs(y - pad + i);
							int core_x = abs(x - pad + j);

							if (core_x >= src.GetWidth()) core_x = src.GetWidth() - (core_x - src.GetWidth()) - 1;
							if (core_y >= src.GetHeight()) core_y = src.GetHeight() - (core_y - src.GetHeight()) - 1;

							const uint8_t* color = src.GetColor(core_x, core_y);
							for (int channel = 0; channel < channels; channel++)
							{
								res_color[channel] += color[channel] * kernel[i][j];
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

}