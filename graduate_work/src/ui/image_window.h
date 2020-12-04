#pragma once

#include "bubble.h"

struct ImageWindow : Module
{
	Ref<Texture2D> mImage;

	ImageWindow(const Ref<Texture2D>& image)
		: mImage(image)
	{}

	void Draw(DeltaTime dt)
	{
		std::string name = "Image" + std::to_string(mImage->mRendererID);
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(name.c_str(), &mIsOpen);
		{
			ImVec2 size = ImGui::GetContentRegionAvail();
			ImGui::Image((void*)mImage->mRendererID, size);
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}
};