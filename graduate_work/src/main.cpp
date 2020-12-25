
#include "bubble_entry_point.h"
#include "image_processing.h"

#include "main_window.h"
#include "image_window.h"

std::vector<uint8_t> rand_color()
{
	std::vector<uint8_t> res;

	for (int i = 0; i < 4; i++)
		res.push_back(std::rand() % 255);
	
	return res;
}


struct MyApplication : Application
{
	MyApplication()
		: Application("Image segmentation")
	{}

	void OnCreate()
	{
		cpu::Image image("resources/images/car2.jpg");

		auto distance_rate = [](const Pixel& a, const Pixel& b)
		{
			float res = 0;
			res += pow(a.x - b.x, 2);
			res += pow(a.y - b.y, 2);

			float brightness1 = 0;
			float brightness2 = 0;

			for (int i = 0; i < b.channels; i++)
			{
				res += 10 * pow(a.color[i] - b.color[i], 2);
				brightness1 += a.color[i];
				brightness2 += b.color[i];
			}
			//res += pow(brightness2 - brightness1, 2);

			return std::sqrt(res);
		};

		auto clusters = ImageKMeans(image, distance_rate, 10, 100);
		cpu::Image image2(image.mSpecification);

		std::vector<std::vector<uint8_t>> c;
		for (int i = 0; i < clusters.size(); i++)
			c.push_back(rand_color());

		for (int i = 0; i < clusters.size(); i++)
		{
			for (Pixel& pixel : clusters[i])
			{
				//uint8_t color[4];
				//color[0] = clusters[i].mCenter.color[0];
				//color[1] = clusters[i].mCenter.color[1];
				//color[2] = clusters[i].mCenter.color[2];
				auto color = c[i];
				image2.SetColor(color.data(), pixel.x, pixel.y);
			}

			uint8_t color[4] = { 255, 0, 0, 0 };
			for (int i = 0; i < clusters.size(); i++)
				for (int k = 0; k < 15; k++)
					for (int j = 0; j < 15; j++)
						image2.SetColor(color, clusters[i].mCenter.x + k, clusters[i].mCenter.y + j);

			for (int i = 0; i < clusters.size(); i++)
				for (int k = 0; k < 5; k++)
					for (int j = 0; j < 5; j++)
					{
						uint8_t color[4];
						//color[0] = clusters[i].mCenter.color[0];
						//color[1] = clusters[i].mCenter.color[1];
						//color[2] = clusters[i].mCenter.color[2];

						image2.SetColor(c[i].data(), clusters[i].mCenter.x + k + 5, clusters[i].mCenter.y + j + 5);
					}

		}

		//Kernel kernel = CreateSharpnessFilter(3, 40);
		//image = cpu::ApplyKernel(kernel, image);
		UI::AddModule<ImageWindow>(image2.LoadOnGPU());
	}

};

Application* CreateApplication()
{
	return new MyApplication;
}
