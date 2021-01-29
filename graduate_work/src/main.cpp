
#include "bubble_entry_point.h"

#include "gen_alg.h"
#include "image_processing.h"

#include "main_window.h"
#include "image_window.h"
#include "selectible_image_window.h"


struct MyApplication : Application
{
	MyApplication()
		: Application("Image segmentation")
	{}

	void OnCreate()
	{
		void meanshift_test();
		void gen_alg_test();


		UI::AddModule<SelectibleImageWindow>(gpu::Image(glm::vec4(1.0f)));

		//gen_alg_test();
		//meanshift_test();
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
	std::vector<uint8_t> res;

	for (int i = 0; i < 4; i++)
		res.push_back(std::rand() % 255);

	res[3] = 255;
	return res;
}


std::vector<Pixel> GetPixels(const cpu::Image& image)
{
	uint32_t image_size = image.GetHeight() * image.GetHeight();
	std::vector<Pixel> pixels;
	pixels.reserve(image_size);

	for (int y = 0; y < image.GetHeight(); y++)
	{
		for (int x = 0; x < image.GetWidth(); x++)
		{
			Pixel pixel = {x, y};
			const uint8_t* color = image.GetColor(x, y);
			for (int i = 0; i < image.GetChannels(); i++)
			{
				pixel.color[i] = color[i];
			}
			pixels.push_back(pixel);
		}
	}

	return std::move(pixels);
}


void meanshift_test()
{
	cpu::Image input("resources/images/lenna.jpg");
	gpu::Image src_image(input);
	//gpu::Image dst_image(src_image.GetWidth() / 2, src_image.GetHeight() / 2);
	gpu::Image dst_image(600, 480);

	Renderer::DrawTexture2D(src_image, dst_image);
	cpu::Image image(dst_image);

	
	cpu::Image image2(image.mSpecification);
	std::vector<Pixel> pixels = GetPixels(image);
	std::vector<Pixel> shifted(pixels.size());
	std::vector<Pixel> shifted_debug(pixels.size());
	
	UI::AddModule<ImageWindow>(image.LoadOnGPU());
	
	// CPU
	//auto clusters = MeanShift::Run(pixels, 10, 3);
	
	// OpenCL
	// ==========================================================
	// Algorithm params
	float radius = 120.0f;
	int iterations = 30;
	
	auto kernel_name = "MeanShift";
	//auto kernel_path = "resources/kernels/test.cl";
	auto kernel_path = "C:/Users/lol/Desktop/projects/GraduateWork/graduate_work/resources/kernels/mean_shift.cl";
	auto [kernel, context, device] = CreateKernel(kernel_path, kernel_name, DeviceType::GPU);
	
	cl_int err = 0;
	uint32_t active_buffer_id = 0;
	uint32_t sub_buffer_id = 1;
	
	cl::Buffer buffer1(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Pixel) * pixels.size(), pixels.data(), &err);
	cl::Buffer buffer2(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY, sizeof(Pixel) * pixels.size(), nullptr, &err);
	cl::Buffer buffers[2] = { buffer1, buffer2 };
	
	cl::CommandQueue queue(context, device);
	
	for (int i = 0; i < iterations; i++)
	{
		clcall(kernel.setArg(0, buffers[active_buffer_id]));
		clcall(kernel.setArg(1, buffers[sub_buffer_id]));
		clcall(kernel.setArg(2, (int)pixels.size()));
		clcall(kernel.setArg(3, radius));
	
		int parts = ceil((float)pixels.size() / 400'000);
		int step_size = pixels.size() / parts;
		
		for (int j = 0; j < parts; j++)
		{
			clcall(queue.enqueueNDRangeKernel(kernel, cl::NDRange(step_size * j), cl::NDRange(pixels.size() / parts)));
		}
		cl::finish();
		std::swap(active_buffer_id, sub_buffer_id);
	
		//============== Snapshot for each step ============== 
		clcall(queue.enqueueReadBuffer(buffers[active_buffer_id], GL_TRUE, 0, sizeof(Pixel) * pixels.size(), shifted.data()));
		
		cpu::Image image_temp(image.mSpecification);
		for (auto& pixel : shifted)
		{
			uint8_t color[] = { pixel.color[0], pixel.color[1], pixel.color[2], 255 };
			image_temp.SetColor(color, pixel.x, pixel.y);
		}
		UI::AddModule<ImageWindow>(image_temp.LoadOnGPU());
		//===========================================
	}
	
	//clcall(queue.enqueueReadBuffer(buffers[active_buffer_id], GL_TRUE, 0, sizeof(Pixel) * pixels.size(), shifted.data(), &events));
	//for (auto& pixel : shifted)
	//{
	//	uint8_t color[] = { pixel.color[0], pixel.color[1], pixel.color[2] };
	//	image2.SetColor(color, pixel.x, pixel.y);
	//}
	//UI::AddModule<ImageWindow>(image2.LoadOnGPU());
	
	
	auto clusters = MeanShift::CreateClusters(pixels, shifted, radius);
	// ==========================================================
	
	std::vector<std::vector<uint8_t>> c;
	srand(time(0));
	
	for (int i = 0; i < clusters.size(); i++)
	{
		c.push_back(rand_color());
	}
	
	for (int i = 0; i < clusters.size(); i++)
	{
		for (Pixel& pixel : clusters[i])
		{
			auto color = c[i];
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
