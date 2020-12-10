
#include "bubble_entry_point.h"

#include "kernel.h"
#include "box_filter.h"
#include "gauss_filter.h"
#include "sharpness_filter.h"

#include "main_window.h"
#include "image_window.h"

void print_kernel(const Kernel& kernel)
{
	for (int i = 0; i < kernel.GetHeight(); i++)
	{
		for (int j = 0; j < kernel.GetWidth(); j++)
		{
			printf("%0.3f ", kernel[i][j]);
		}
		printf("\n");
	}
}


struct MyApplication : Application
{
	MyApplication()
		: Application("Image segmentation")
	{}

	void OnCreate()
	{
		Image image("resources/images/nature.jpg");
		UI::AddModule<ImageWindow>(Renderer::CopyTexture2D(image));

		Kernel kernel = CreateSharpnessFilter(3);

		Image res = kernel.Apply(image);
		UI::AddModule<ImageWindow>(std::move(res));

		// OpenCL temp
		//std::vector<cl::Platform> platforms;
		//cl::Platform::get(&platforms);
		//
		//std::vector<cl::Device> devices;
		//platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
		//
		//cl::string name = devices[0].getInfo<CL_DEVICE_VENDOR>();
	}

	void OnUpdate(DeltaTime dt)
	{

	}

	void OnEvent(const SDL_Event& event)
	{

	}

};

Application* CreateApplication()
{
	return new MyApplication;
}