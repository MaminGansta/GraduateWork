
#include "bubble.h"
#include "bubble_entry_point.h"

#include "kernel.h"
#include "main_window.h"
#include "image_window.h"


struct MyApplication : Application
{
	MyApplication()
		: Application("Image segmentation")
	{}

	void OnCreate()
	{
		Texture2D image("resources/images/nature.jpg");

		Kernel kernel = { { 0.1f, 0.1f, 0.1f },
						  { 0.1f, 0.1f, 0.1f },
						  { 0.1f, 0.1f, 0.1f } };

		Texture2D res = kernel.Apply(image);
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

	void OnUpdate(Bubble::DeltaTime dt)
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