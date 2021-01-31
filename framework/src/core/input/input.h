#pragma once

#include "glm/glm.hpp"
#include "SDL2/SDL_events.h"

#include "window/window.h"


namespace Bubble
{
	class Input
	{
		static Window* sWindow;
		static short sKeyMap[256];
		static int sMouseKeyMap[16];
		static int sMouseKeyMapLast[16];
		static int sMouseWheelOffset;

		static int sMousePosX;
		static int sMousePosY;
		static int sMouseRelPosX;
		static int sMouseRelPosY;

	public:
		static void SetWindow(Window* window);
		static void OnEvent(const SDL_Event& event);
		
		// Codes: SDLK_a, SDLK_1, ...
		static bool IsKeyPressed(SDL_Keycode code);
		// Codes: SDLK_a, SDLK_1, ...
		static bool IsKeyClicked(SDL_Keycode code);

		/*
			Codes:
			SDL_BUTTON_LEFT
			SDL_BUTTON_MIDDLE
			SDL_BUTTON_RIGHT
			SDL_BUTTON_X1
			SDL_BUTTON_X2

			@return num of fast clicks before press
		*/
		static int IsMouseButtonPressed(int button);

		/*
			Codes:
			SDL_BUTTON_LEFT
			SDL_BUTTON_MIDDLE
			SDL_BUTTON_RIGHT
			SDL_BUTTON_X1
			SDL_BUTTON_X2

			@return num of fast clicks
		*/
		static int IsMouseButtonClicked(int button);

		static glm::ivec2 GetMousePosition();
		static int GetMouseX();
		static int GetMouseY();

		static glm::ivec2 GetMouseRelPosition();
		static int GetMouseRelX();
		static int GetMouseRelY();

		// return values between 0.0f and 1.0f
		static glm::vec2 fGetMousePosition();
		static float fGetMouseX();
		static float fGetMouseY();

		static glm::vec2 fGetMouseRelPosition();
		static float fGetMouseRelX();
		static float fGetMouseRelY();

		static int GetMouseWheelOffset();

		// Flush all relative information
		static void NewFrame();
	};
}