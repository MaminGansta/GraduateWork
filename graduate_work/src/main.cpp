
#include "bubble.h"

#include "ui/main_window.h"
#include "UI/image_window.h"


struct MyApplication : Application
{
	MyApplication()
		: Application("Image segmentation")
	{}

	void OnCreate()
	{
		UI::AddModule(CreateScope<TestWindow>());

		Ref<Texture2D> image = CreateRef<Texture2D>("resources/images/nature.jpg");
		UI::AddModule(CreateScope<ImageWindow>(image));

		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);

		std::vector<cl::Device> devices;
		platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);

		cl::string name = devices[0].getInfo<CL_DEVICE_VENDOR>();

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