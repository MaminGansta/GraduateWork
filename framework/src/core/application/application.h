#pragma once

#include "ui.h"
#include "core.h"
#include "renderer.h"

namespace Bubble
{
	struct Application
	{
		static Scope<Window> sWindow;
		Scope<UI> mUI;
		Timer mTimer;

		Application(const std::string& name = "Window");
		virtual void Run();
		virtual void OnCreate() {};
		virtual void OnUpdate(DeltaTime) {};
		virtual void OnEvent(const SDL_Event& event) {};
		virtual ~Application() {};
		static Window& GetWindow() { return *sWindow; };
	};
}