
#include "window.h"


namespace Bubble
{
    Window::Window(const std::string& name)
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            printf("Error: %s", SDL_GetError());
			throw std::runtime_error("Window can't be created");
		}
        // GL 3.3 + GLSL 330
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

        mWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
        mGLContext = SDL_GL_CreateContext(mWindow);

        SDL_GL_MakeCurrent(mWindow, mGLContext);
        SDL_GL_SetSwapInterval(1); // Enable vsync

        if (int error = glewInit(); error != GLEW_OK)
        {
            printf("Error: Glew init \n%s", glewGetErrorString(error));
            throw std::runtime_error("Window can't be created");
        }
    }

	Window::~Window()
	{
		SDL_GL_DeleteContext(mGLContext);
		SDL_DestroyWindow(mWindow);
	}

	bool Window::IsOpen()
	{
        return mIsOpen;
	}

	int Window::GetWidth()
    {
        int width = 0, height = 0;
        SDL_GetWindowSize(mWindow, &width, &height);
        return width;
    }

    int Window::GetHeight()
    {
		int width = 0, height = 0;
        SDL_GetWindowSize(mWindow, &width, &height);
        return height;
    }

	glm::ivec2 Window::GetSize()
	{
		int width = 0, height = 0;
		SDL_GetWindowSize(mWindow, &width, &height);
        return glm::ivec2(width, height);
	}

	SDL_Window* Window::GetSDLWindow()
    {
        return mWindow;
    }

    SDL_GLContext Window::GetGLContext()
    {
        return mGLContext;
    }

    const char* Window::GetGLSLVersion()
    {
        return glsl_version;
    }

    bool Window::PollEvent(SDL_Event& event)
    {
        return SDL_PollEvent(&event);
    }

    void Window::OnEvent(const SDL_Event& event)
    {
        if (event.type == SDL_QUIT)
        {
            mIsOpen = false;
        }
        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(mWindow))
        {
            mIsOpen = false;
        }
    }

    void Window::OnUpdate()
    {
        SDL_GL_SwapWindow(mWindow);
    }

    void Window::Close()
    {
        mIsOpen = false;
    }

	void Window::SetVSync(bool mode)
	{
        SDL_GL_SetSwapInterval(mode);
	}

}