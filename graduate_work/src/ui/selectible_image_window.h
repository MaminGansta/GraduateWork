#pragma once

#include "bubble.h"


struct SelectibleImageWindow : Module
{
	Ref<Texture2D> mImage;
	Ref<Framebuffer> mCanvas = CreateRef<Framebuffer>(100, 100);

	SelectibleImageWindow(const Ref<Texture2D>& image)
		: mImage(image)
	{}

	SelectibleImageWindow(Texture2D&& image)
		: mImage(CreateRef<Texture2D>(std::move(image)))
	{}

	void Draw(DeltaTime dt)
	{
		std::string name = "Image" + std::to_string(mImage->mRendererID);
		//ImGui::ShowDemoWindow();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(name.c_str(), &mIsOpen);
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			const ImVec2 p = ImGui::GetCursorScreenPos();
			float x = p.x + 20.0f, y = p.y + 20.0f;
			float sz = 30;

			static ImVec4 colf = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			const ImU32 col = ImColor(colf);

			const ImDrawCornerFlags corners_none = 0;
			const ImDrawCornerFlags corners_all = ImDrawCornerFlags_All;
			const ImDrawCornerFlags corners_tl_br = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotRight;

			ImVec2 size = ImGui::GetContentRegionAvail();
			ImGui::Image((void*)mImage->GetRendererID(), size);
			
			//draw_list->AddRect(ImVec2(x, y), ImVec2(x + 20, y + 20), col, 10.0f, corners_all, 3.0f);
			draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col);
		}
		ImGui::End();
		ImGui::PopStyleVar();


		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		//ImGui::Begin(name.c_str(), &mIsOpen);
		//{
		//	ImVec2 size = ImGui::GetContentRegionAvail();
		//	ImGui::Image((void*)mImage->mRendererID, size);
		//}
		//ImGui::End();
		//ImGui::PopStyleVar();
	}
};