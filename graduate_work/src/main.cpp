
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