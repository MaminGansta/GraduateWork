
#include "bubble_entry_point.h"

#include "gen_alg.h"
#include "image_processing.h"
#include "gpu/meanshift/meanshift_gpu.h"

#include "main_window.h"
#include "image_window.h"
#include "selectible_image_window.h"


struct MyApplication : Application
{
	Ref<SelectibleImageWindow> selectible_window;

	MyApplication()
		: Application("Image segmentation")
	{}

	void OnCreate()
	{
		void meanshift_test(const Ref<SelectibleImageWindow>&);
		void gen_alg_test();

		//Ref<Texture2D> image = CreateRef<Texture2D>("resources/images/lenna.jpg");
		//UI::AddModule<MainWindow>(image);

		//gen_alg_test();
		meanshift_test(selectible_window);
	}

};

Application* CreateApplication()
{
	return new MyApplication;
}


// ====================== Test area ====================== 


struct MeanShiftBreed
{
	Ref<RNG> mRNG = CreateRef<RNG>();
	uint8_t mGens[10] = {0};

	static inline float GetTargetValue(MeanShiftBreed entity)
	{
		return 0.0f;
	}

	static inline MeanShiftBreed Crossover(const MeanShiftBreed& a, const MeanShiftBreed& b)
	{ 
		return a;
	}

	static inline MeanShiftBreed Mutation (const MeanShiftBreed& a)
	{ 
		return a;
	}
};

void gen_alg_test()
{
	std::vector<MeanShiftBreed> a(5);
	GeneticAlgorithm(a, 10);
}



// ====================== MeanShift ====================== 

std::vector<uint8_t> rand_color()
{
	RNG rng;
	std::vector<uint8_t> res;

	for (int i = 0; i < 4; i++)
		res.push_back(rng.Get<uint8_t>());

	res[3] = 255;
	return res;
}



void meanshift_test(const Ref<SelectibleImageWindow>& selectible_window)
{
	cpu::Image input("resources/images/lenna.jpg");
	gpu::Image src_image(input);
	gpu::Image dst_image(600, 480);

	Renderer::DrawTexture2D(src_image, dst_image);
	cpu::Image image(dst_image);

	
	// CPU
	//auto clusters = MeanShift::Run(pixels, 10, 3);
	
	MeanShitParams params;
	params.Radius = 120.0f;
	params.DistanceCoef = 3;
	params.ColorCoef = 12;
	params.BrightnessCoef = 1;
	params.Iterations = 15;

	gpu::MeanShift meanshift;
	std::vector<std::vector<Pixel>> snapshots;

    // OpenCL
	auto clusters = meanshift.Run(image, params, &snapshots);
	
	// Draw snapshots
	for (const auto& snapshot : snapshots)
	{
		cpu::Image snapshot_image(image.mSpecification);
		for (auto& pixel : snapshot)
		{
			uint8_t color[] = { pixel.color[0], pixel.color[1], pixel.color[2] };
			snapshot_image.SetColor(color, pixel.x, pixel.y);
		}
		UI::AddModule<ImageWindow>(snapshot_image.LoadOnGPU());
	}
	

	// Draw clusters
	std::vector<std::vector<uint8_t>> colors;
	for (int i = 0; i < clusters.size(); i++)
	{
		colors.push_back(rand_color());
	}
	
    cpu::Image image2(image.mSpecification);
	for (int i = 0; i < clusters.size(); i++)
	{
		for (Pixel& pixel : clusters[i])
		{
			auto color = colors[i];
			image2.SetColor(color.data(), pixel.x, pixel.y);
		}
	}
	UI::AddModule<ImageWindow>(image2.LoadOnGPU());
}


//void kmeans_test()
//{
//	cpu::Image image("resources/images/car2.jpg");
//
//	auto distance_rate = [](const Pixel& a, const Pixel& b)
//	{
//		float res = 0;
//		res += pow(a.x - b.x, 2);
//		res += pow(a.y - b.y, 2);
//
//		float brightness1 = 0;
//		float brightness2 = 0;
//
//		for (int i = 0; i < b.channels; i++)
//		{
//			res += 10 * pow(a.color[i] - b.color[i], 2);
//			brightness1 += a.color[i];
//			brightness2 += b.color[i];
//		}
//		//res += pow(brightness2 - brightness1, 2);
//
//		return std::sqrt(res);
//	};
//
//	auto clusters = ImageKMeans(image, distance_rate, 10, 100);
//	cpu::Image image2(image.mSpecification);
//
//	std::vector<std::vector<uint8_t>> c;
//	for (int i = 0; i < clusters.size(); i++)
//		c.push_back(rand_color());
//
//	for (int i = 0; i < clusters.size(); i++)
//	{
//		for (Pixel& pixel : clusters[i])
//		{
//			//uint8_t color[4];
//			//color[0] = clusters[i].mCenter.color[0];
//			//color[1] = clusters[i].mCenter.color[1];
//			//color[2] = clusters[i].mCenter.color[2];
//			auto color = c[i];
//			image2.SetColor(color.data(), pixel.x, pixel.y);
//		}
//
//		uint8_t color[4] = { 255, 0, 0, 0 };
//		for (int i = 0; i < clusters.size(); i++)
//			for (int k = 0; k < 15; k++)
//				for (int j = 0; j < 15; j++)
//					image2.SetColor(color, clusters[i].mCenter.x + k, clusters[i].mCenter.y + j);
//
//		for (int i = 0; i < clusters.size(); i++)
//			for (int k = 0; k < 5; k++)
//				for (int j = 0; j < 5; j++)
//				{
//					uint8_t color[4];
//					//color[0] = clusters[i].mCenter.color[0];
//					//color[1] = clusters[i].mCenter.color[1];
//					//color[2] = clusters[i].mCenter.color[2];
//
//					image2.SetColor(c[i].data(), clusters[i].mCenter.x + k + 5, clusters[i].mCenter.y + j + 5);
//				}
//
//	}
//
//	//Kernel kernel = CreateSharpnessFilter(3, 40);
//	//image = cpu::ApplyKernel(kernel, image);
//	UI::AddModule<ImageWindow>(image2.LoadOnGPU());
//}
