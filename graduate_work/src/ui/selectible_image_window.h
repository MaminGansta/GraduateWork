#pragma once

#include "bubble.h"
#include "imgui_toolkit.h"


struct SelectibleImageWindow : Module
{
	Ref<Texture2D> mImage;
    ImVec2 mPointInClass = { 0.5f, 0.5f };
    ImVec2 mCircleCenter = { 0.5f, 0.5f };
	float mRadius = 0.1f;

	SelectibleImageWindow(const Ref<Texture2D>& image)
		: mImage(image)
	{}

	SelectibleImageWindow(Texture2D&& image)
		: mImage(CreateRef<Texture2D>(std::move(image)))
	{}

	void Draw(DeltaTime dt)
	{
		std::string name = "Selectable area";
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(name.c_str(), &mIsOpen);
		{
			// Select circle
			if (ImGui::IsWindowFocused() && Input::IsMouseButtonPressed(SDL_BUTTON_LEFT))
			{
				if (auto mouse_pos = ImGuiTK::GetMousePosInRect())
				{
					if (Input::IsMouseButtonClicked(SDL_BUTTON_LEFT))
					{
						mCircleCenter = mouse_pos.value();
					}
					ImVec2 diff = mCircleCenter - mouse_pos.value();
					mRadius = 0.0f;
					mRadius += pow(diff.x, 2);
					mRadius += pow(diff.y, 2);
					mRadius = sqrtf(mRadius);
				}
			}

			// Move circle
			if (ImGui::IsWindowFocused() && Input::IsMouseButtonPressed(SDL_BUTTON_MIDDLE))
			{
				if (auto mouse_pos = ImGuiTK::GetMousePosInRect())
				{
					mCircleCenter = mouse_pos.value();
				}
			}

			// Select pixel
			if (ImGui::IsWindowFocused() && Input::IsMouseButtonClicked(SDL_BUTTON_RIGHT))
			{
				if (auto mouse_pos = ImGuiTK::GetMousePosInRect())
				{
					mPointInClass = mouse_pos.value();
				}
			}

			ImGuiTK::DrawImage(mImage);
			ImGuiTK::DrawFilledRect(mPointInClass, {0.01f, 0.01f}, { 1.0f, 0.0f, 0.0f, 1.0f });
			ImGuiTK::DrawCircle(mCircleCenter, mRadius, { 1.0f, 0.0f, 0.0f, 1.0f });
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

};