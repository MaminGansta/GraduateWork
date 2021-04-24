#pragma once

#include "bubble.h"

struct ImageWindow : Module
{
	Ref<Texture2D> mImage;
    std::string	   mName;

	ImageWindow(const Ref<Texture2D>& image, const std::string& name = {})
		: mImage(image)
		, mName(name)
	{}

	ImageWindow(Texture2D&& image, const std::string& name = {})
		: mImage(CreateRef<Texture2D>(std::move(image)))
		, mName(name)
	{}

	void Draw(DeltaTime dt)
	{
		std::string name;

		if (mName.empty())
			name = "Image " + std::to_string(mImage->mRendererID);
		else
			name = mName;
		
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