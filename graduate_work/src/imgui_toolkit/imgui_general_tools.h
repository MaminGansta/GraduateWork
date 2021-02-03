#pragma once

#include <optional>
#include "bubble.h"

namespace ImGuiTK
{
    glm::vec2& VecC(const ImVec2& vec) { return *(glm::vec2*)&vec; }
    ImVec2& VecC(const glm::vec2& vec) { return *(ImVec2*)&vec; }

    glm::vec4& VecC(const ImVec4& vec) { return *(glm::vec4*)&vec; }
    ImVec4& VecC(const glm::vec4& vec) { return *(ImVec4*)&vec; }


	// Possible to be outside
	// Use GetMousePosInRect() to get mouse in workable area
	inline ImVec2 GetMousePos()
	{
		return (ImGui::GetMousePos() - ImGui::GetCursorScreenPos()) / ImGui::GetWindowSize();
	}

	inline bool InRect(ImVec2 point, ImVec4 rect = { 0.0f, 0.0f, 1.0f, 1.0f })
	{
		return point.x > 0.0f && point.x < 1.0f
			&& point.y > 0.0f && point.y < 1.0f;
	}


	inline std::optional<ImVec2> GetMousePosInRect(ImVec4 rect = { 0.0f, 0.0f, 1.0f, 1.0f })
	{
		ImVec2 mouse_pos = (ImGui::GetMousePos() - ImGui::GetCursorScreenPos()) / ImGui::GetWindowSize();
		if (InRect(mouse_pos, rect))
		{
			return mouse_pos;
		}
		return {};
	}
}