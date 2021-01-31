#pragma once

#include "bubble.h"

namespace ImGuiTK
{
	inline void DrawImage(const Ref<Texture2D>& image, ImVec2 pos = {0, 0}, ImVec2 size = {1.0f, 1.0f})
	{
		const ImVec2 window_pos = ImGui::GetCursorScreenPos();
		const ImVec2 window_size = ImGui::GetWindowSize();
		pos = window_pos + pos * window_size;
		size = size * window_size;
		ImGui::GetWindowDrawList()->AddImage((void*)image->mRendererID, pos, pos + size);
	}


	inline void DrawFilledRect(ImVec2 pos, ImVec2 size, ImVec4 color = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f })
	{
		const ImVec2 window_pos = ImGui::GetCursorScreenPos();
		const ImVec2 windows_size = ImGui::GetWindowSize();
		ImVec2 top_left = window_pos + windows_size * pos;
		ImVec2 bottom_right = window_pos + windows_size * (pos + size);
		ImGui::GetWindowDrawList()->AddRectFilled(top_left, bottom_right, ImColor(color));
	}

	inline void DrawCircle(ImVec2 center, float radius, ImVec4 color = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f }, float thickness = 1.0f)
	{
		const ImVec2 window_pos = ImGui::GetCursorScreenPos();
		const ImVec2 windows_size = ImGui::GetWindowSize();
		center = window_pos + windows_size * center;
		radius *= windows_size.x;
		ImGui::GetWindowDrawList()->AddCircle(center, radius, ImColor(color), 32, thickness);
	}

}