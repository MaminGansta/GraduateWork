#pragma once

#include "bubble.h"

struct TestWindow : Module
{
	void Draw(DeltaTime dt) override
	{
		ImGui::Begin("TestWindow", &mIsOpen);
		{
			if (ImGui::Button("button", { 100, 100 }))
			{

			}
		}
		ImGui::End();
	}
};