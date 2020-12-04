#pragma once

#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "glm/glm.hpp"

#include <cstdio>
#include <cassert>
#include <stdexcept>


namespace Bubble
{
	class Window
	{
		SDL_Window* mWindow;
		SDL_GLContext mGLContext;
		const char* glsl_version = "#version 330";
		bool mIsOpen = true;

	public:
		Window(const std::string& name = "Window");
		~Window();

		bool IsOpen();

		int GetWidth();
		int GetHeight();
		glm::ivec2 GetSize();

		SDL_Window* GetSDLWindow();
		SDL_GLContext GetGLContext();
		const char* GetGLSLVersion();

		bool PollEvent(SDL_Event& event);
		void OnEvent(const SDL_Event& event);

		void OnUpdate();
		void Close();

		void SetVSync(bool mode);
	};

}