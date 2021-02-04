#pragma once

#include "bubble.h"
#include "imgui_drawing_tools.h"
#include "selectible_image_window.h"
#include "image_gallerey.h"
#include "image_window.h"

#include "file_dialog.h"
#include "image_processing.h"
#include "bread.h"
#include "gen_alg.h"

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
					Ref<gpu::MeanShift> meanshift = CreateRef<gpu::MeanShift>();
					Ref<std::vector<Pixel>> pixels = CreateRef<std::vector<Pixel>>(GetPixels(*mImage));

                    float radius = mSelectibleImageWindow->mRadius * mImage->GetWidth();
                    ImVec2 center = mSelectibleImageWindow->mCircleCenter;
                    center.x = floor(center.x * mImage->GetWidth());
                    center.y = floor(center.y * mImage->GetHeight());

					std::vector<MeanShiftBreed> population;
					for (int i = 0; i < 5; i++)
					{
						MeanShiftBreed bread(pixels, meanshift, center, radius);
						bread.ClipValues();
						population.push_back(bread);
					}

					auto result = GeneticAlgorithm(population, 6);

					// Print result
					std::vector<std::vector<Pixel>> snapshots;
                    MeanShitParams params = ExtractParams(result.back()[0].mGens);

					LOG_INFO("Params:\n radius {}\n distance coef{}\n color coef{}\n brightness coef{}",
								params.Radius, params.DistanceCoef, params.ColorCoef, params.BrightnessCoef);

					auto clusters = meanshift->Run(*pixels, params, &snapshots);
					
					// Draw snapshots
					UI::AddModule<ImageGralleryWindow>(GetRefImagesFromPixelData(snapshots, mImage->mSpecification));
					
					// Draw clusters
					UI::AddModule<ImageWindow>(GetImageFromClusters(clusters, mImage->mSpecification));
					
					int evaluation = MeanshiftEvaluation(clusters, center, radius);
					LOG_INFO("Evalueation: {}", evaluation);
                }

			}
		}
		ImGui::End();
	}

};