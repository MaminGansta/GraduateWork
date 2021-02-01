#pragma once

#include "bubble.h"
#include "imgui_drawing_tools.h"
#include "selectible_image_window.h"


struct MainWindow : Module
{
	Ref<Texture2D> mImage;
	Ref<SelectibleImageWindow> mSelectibleImageWindow;

	// Temp
	MainWindow(const Ref<Texture2D>& image)
		: mImage(image),
		  mSelectibleImageWindow(CreateRef<SelectibleImageWindow>(image))
	{
		mWindowFlags |= ImGuiWindowFlags_NoCollapse;
	}

	void Draw(DeltaTime dt) override
	{
		ImGui::Begin("Main window", NULL, mWindowFlags);
		{
			if (ImGui::Button("Stage 1: Choose image", { 200, 50 }))
			{

			} 
			
			if (mImage)
			{
				ImGui::SameLine(0.0f, 20.0f);
				ImGui::Image((ImTextureID)mImage->GetRendererID(), { 100.0f, 100.0f });
			}

			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			if (ImGui::Button("Stage 2: Select area", { 200, 50 }))
			{
				mSelectibleImageWindow->mIsOpen = true;
				mSelectibleImageWindow->mImage = mImage;
				UI::AddModule(mSelectibleImageWindow);
			}



		}
		ImGui::End();
	}

};