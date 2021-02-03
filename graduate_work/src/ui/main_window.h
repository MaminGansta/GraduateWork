#pragma once

#include "bubble.h"
#include "imgui_drawing_tools.h"
#include "selectible_image_window.h"
#include "image_gallerey.h"
#include "image_window.h"

#include "file_dialog.h"
#include "image_processing.h"
#include "bread.h"

struct MainWindow : Module
{
	Ref<Texture2D> mImage;
	Ref<SelectibleImageWindow> mSelectibleImageWindow;

	MainWindow()
	{
		mWindowFlags |= ImGuiWindowFlags_NoCollapse;
	}

	void Draw(DeltaTime dt) override
	{
		ImGui::Begin("Main window", NULL, mWindowFlags);
		{
			if (ImGui::Button("Stage 1: Choose image", { 200, 50 }))
			{
				try {
                    std::string image_path = OpenFileDialog("jpg,png");
					mImage = CreateRef<Texture2D>(Renderer::ResizeTexture2D(cpu::Image(image_path), 400, 320));
					mSelectibleImageWindow = CreateRef<SelectibleImageWindow>(mImage);
				}
				catch (std::exception& e) {
					LOG_WARN(e.what());
				}
			}
			
			if (mImage)
			{
				ImGui::SameLine(0.0f, 20.0f);
				ImGui::Image((ImTextureID)mImage->GetRendererID(), { 100.0f, 100.0f });

				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				if (ImGui::Button("Stage 2: Select area", { 200, 50 }))
				{
					mSelectibleImageWindow->mIsOpen = true;
					UI::AddModule(mSelectibleImageWindow);
				}

                ImGui::Dummy(ImVec2(0.0f, 5.0f));
                if (ImGui::Button("Stage 3: Run", { 200, 50 }))
                {
					//MeanShitParams params;
					//params.Radius = 110;
					//params.DistanceCoef = 12;
					//params.ColorCoef = 4;
					//params.BrightnessCoef = 12;
					//params.Iterations = 16;
					
					// OpenCL
					//gpu::MeanShift meanshift;
					//std::vector<std::vector<Pixel>> snapshots;
					//
					//auto clusters = meanshift.Run(*mImage, params, &snapshots);
					//
					//// Draw snapshots
					//UI::AddModule<ImageGralleryWindow>(GetRefImagesFromPixelData(snapshots, mImage->mSpecification));
					//
					//// Draw clusters
					//UI::AddModule<ImageWindow>(GetImageFromClusters(clusters, mImage->mSpecification));
					//float radius = mSelectibleImageWindow->mRadius * mImage->GetWidth();
					//ImVec2 center = mSelectibleImageWindow->mCircleCenter;
					//center.x = floor(center.x * mImage->GetWidth());
					//center.y = floor(center.y * mImage->GetHeight());
					//
					//int evaluation = MeanshiftEvaluation(clusters, center, radius);
					//LOG_INFO("Evalueation: {}", evaluation);
                }

			}
		}
		ImGui::End();
	}

};