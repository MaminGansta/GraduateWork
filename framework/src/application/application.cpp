
#include "application.h"


namespace Bubble
{
    Scope<Window> Application::sWindow = nullptr;

	Application::Application(const std::string& name)
    {
        sWindow = CreateScope<Window>(name);
		mUI = CreateScope<UI>(sWindow.get());
		OnCreate();
    }

	void Application::Run()
	{
		Renderer::Init();
		Bubble::Log::init();
		Bubble::Input::SetWindow(sWindow.get());
		OnCreate();

		while (sWindow->IsOpen())
		{
			SDL_Event event;
			while (sWindow->PollEvent(event))
			{
				OnEvent(event);
				sWindow->OnEvent(event);
				Input::OnEvent(event);
			}
			OnUpdate(mTimer.GetDeltaTime());
			mUI->OnUpdate(mTimer.GetDeltaTime());
			sWindow->OnUpdate();
			mTimer.Update();
		}
	}

}