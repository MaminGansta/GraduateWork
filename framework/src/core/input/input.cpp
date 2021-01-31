
#include "input.h"


namespace Bubble
{
	Window* Input::sWindow;
	short Input::sKeyMap[256];
	int Input::sMouseKeyMap[16];
	int Input::sMouseKeyMapLast[16];
	int Input::sMouseWheelOffset;

	int Input::sMousePosX;
	int Input::sMousePosY;
	int Input::sMouseRelPosX;
	int Input::sMouseRelPosY;

	void Input::SetWindow(Window* window)
	{
		sWindow = window;
	}

	void Input::OnEvent(const SDL_Event& event)
	{
		switch (event.type)
		{
			case SDL_KEYUP:
				if (event.key.keysym.sym >= 1073741881) {
					sKeyMap[event.key.keysym.sym - 1073741881] = 0;
				}
				else if (event.key.keysym.sym < 128) {
					sKeyMap[event.key.keysym.sym] = 0;
				}
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym >= 1073741881) {
					sKeyMap[event.key.keysym.sym - 1073741881] = 1 + event.key.repeat;
				}
				else if (event.key.keysym.sym < 128) {
					sKeyMap[event.key.keysym.sym] = 1 + event.key.repeat;
				}
				break;

			case SDL_MOUSEWHEEL:
				sMouseWheelOffset = event.wheel.y;
				break;

			case SDL_MOUSEMOTION:
				sMousePosX = event.motion.x;
				sMousePosY = event.motion.y;
				sMouseRelPosX = event.motion.xrel;
				sMouseRelPosY = event.motion.yrel;
				break;

			case SDL_MOUSEBUTTONDOWN:
				sMouseKeyMap[event.button.button] = event.button.clicks;
				break;

			case SDL_MOUSEBUTTONUP:
				sMouseKeyMap[event.button.button] = 0;
				break;
			}
		
	}

	bool Input::IsKeyPressed(SDL_Keycode code)
	{
		if (code >= 1073741881) {
			code -= 1073741881;
		}
		return sKeyMap[code];
	}

	bool Input::IsKeyClicked(SDL_Keycode code)
	{
		if (code >= 1073741881) {
			code -= 1073741881;
		}
		return sKeyMap[code] == 1;
	}

	int Input::IsMouseButtonPressed(int button)
	{
		return sMouseKeyMap[button];
	}

	int Input::IsMouseButtonClicked(int button)
	{
		if (sMouseKeyMap[button] && !sMouseKeyMapLast[button])
		{
			return sMouseKeyMap[button];
		}
		else {
			return 0;
		}
	}

	glm::ivec2 Input::GetMousePosition()
	{
		return glm::ivec2(sMousePosX, sMousePosY);
	}

	glm::vec2 Input::fGetMousePosition()
	{
		glm::vec2 window_size = sWindow->GetSize();
		return glm::vec2((float)sMousePosX / window_size.x, (float)sMousePosY / window_size.y);
	}

	float Input::fGetMouseX()
	{
		int width = sWindow->GetWidth();
		return (float)sMousePosX / width;
	}

	float Input::fGetMouseY()
	{
		int height = sWindow->GetHeight();
		return (float)sMousePosY / height;
	}

	glm::vec2 Input::fGetMouseRelPosition()
	{
		glm::vec2 window_size = sWindow->GetSize();
		return glm::vec2((float)sMouseRelPosX / window_size.x, (float)sMouseRelPosY / window_size.y);
	}

	float Input::fGetMouseRelX()
	{
		int width = sWindow->GetWidth();
		return (float)sMouseRelPosX / width;
	}

	float Input::fGetMouseRelY()
	{
		int height = sWindow->GetHeight();
		return (float)sMouseRelPosY / height;
	}

	int Input::GetMouseX()
	{
		return sMousePosX;
	}

	int Input::GetMouseY()
	{
		return sMousePosY;
	}

	glm::ivec2 Input::GetMouseRelPosition()
	{
		return glm::ivec2(sMouseRelPosX, sMouseRelPosY);
	}

	int Input::GetMouseRelX()
	{
		return sMouseRelPosX;
	}

	int Input::GetMouseRelY()
	{
		return sMouseRelPosY;
	}

	int Input::GetMouseWheelOffset()
	{
		return sMouseWheelOffset;
	}

	void Input::NewFrame()
	{
		sMouseRelPosX = 0.0f;
		sMouseRelPosY = 0.0f;
		sMouseWheelOffset = 0.0f;
		memmove(sMouseKeyMapLast, sMouseKeyMap, sizeof(sMouseKeyMap));
	}

}