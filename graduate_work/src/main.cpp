
#include "bubble.h"

#include "ui/test_window.h"


struct MyApplication : Bubble::Application
{
	void OnCreate()
	{
		GetWindow().SetVSync(false);
		Bubble::UI::AddModule(Bubble::CreateScope<TestWindow>());
	}

	void OnUpdate(Bubble::DeltaTime dt)
	{

	}

	void OnEvent(const SDL_Event& event)
	{

	}
};

Bubble::Application* CreateApplication()
{
	return new MyApplication;
}