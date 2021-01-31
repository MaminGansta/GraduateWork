
#include "application.h"


namespace Bubble
{
    Scope<Window> Application::sWindow = nullptr;

	Application::Application(const std::string& name)
    {
        sWindow = CreateScope<Window>(name);
		mUI = CreateScope<UI>(sWindow.get()); 
    }

	void Application::Run()
	{
		Renderer::Init(RenderingMod::ImageProccessing);
		Bubble::Log::init();
		Bubble::Input::SetWindow(sWindow.get());
		OnCreate();

		while (sWindow->IsOpen())
		{
			Input::NewFrame();
			
			SDL_Event event;
			while (sWindow->PollEvent(event))
			{
				OnEvent(event);
				sWindow->OnEvent(event);
				Input::OnEvent(event);
				mUI->mImGuiControll.OnEvent(event);
			}
			
			this->OnUpdate(mTimer.GetDeltaTime());
			mUI->OnUpdate(mTimer.GetDeltaTime());
			sWindow->OnUpdate();
			mTimer.OnUpdate();
		}
	}

}