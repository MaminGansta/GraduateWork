#pragma once

#include "window/window.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"


namespace Bubble
{
    class ImGuiControll
    {
		Window* mWindow;

    public:
        ImGuiControll() = default;
        ImGuiControll(Window* window);

        void OnAttach();
        void OnDetach();

        void Begin(); // Imgui scope
        void End();

		void BeginMenuBar();
		void EndMenuBar();

        void OnEvent(SDL_Event& event);

    private:
		void ImGuiMultiViewports();
    };
}