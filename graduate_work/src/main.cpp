
#include "bubble_entry_point.h"

#include "image_processing.h"

#include "main_window.h"
#include "image_window.h"


struct MyApplication : Application
{
	MyApplication()
		: Application("Image segmentation")
	{}

	void OnCreate()
	{
		//Texture2D image("resources/images/nature.jpg");

		cpu::Image image("resources/images/nature.jpg");

		//UI::AddModule<ImageWindow>(Renderer::CopyTexture2D(image));

		Kernel kernel = CreateSharpnessFilter(3, 40);

		//Texture2D res = kernel.ApplyGPU(image);
		//UI::AddModule<ImageWindow>(std::move(res));

		image = cpu::ApplyKernel(kernel, image);

		UI::AddModule<ImageWindow>(image.LoadOnGPU());

		// OpenCL temp
		//std::vector<cl::Platform> platforms;
		//cl::Platform::get(&platforms);
		//
		//std::vector<cl::Device> devices;
		//platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
		//
		//cl::string name = devices[0].getInfo<CL_DEVICE_VENDOR>();
	}

};

Application* CreateApplication()
{
	return new MyApplication;
}