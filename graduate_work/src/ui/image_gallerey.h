#pragma once

#include "bubble.h"


struct ImageGalleryWindow : Module
{
	std::vector<Ref<Texture2D>> mImages;

	ImageGalleryWindow() = default;

	ImageGalleryWindow(std::vector<Ref<Texture2D>> images)
		: mImages(std::move(images))
	{}

	void SetImages(std::vector<Ref<Texture2D>> images)
	{
		mImages = std::move(images);
	}

	void Draw(DeltaTime dt)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Meanshift steps", &mIsOpen);
		{
			ImVec2 size = ImGui::GetContentRegionAvail();
			for (int i = 0; i < mImages.size(); i++)
			{
				ImGui::Text("%d", i);
				ImGui::Image((void*)mImages[i]->mRendererID, size);
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

};