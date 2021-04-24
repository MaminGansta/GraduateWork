#pragma once

#include "bubble.h"
#include "imgui_drawing_tools.h"
#include "selectible_image_window.h"
#include "image_gallerey.h"
#include "image_window.h"

#include "file_dialog.h"
#include "image_processing.h"
#include "breed.h"
#include "gen_alg.h"


double RateDitailsOnImage(const cpu::Image& image)
{
	Kernel kernel_sobel_horizontal{
		{ -1,  0,  1 },
		{ -2,  0,  2 },
		{ -1,  0,  1 }
	};
	Kernel kernel_sobel_vertical{
		{ -1, -2, -1 },
		{  0,  0,  0 },
		{  1,  2,  1 }
	};

	auto bw    = cpu::RGB2YCbCr(image);
	auto h_res = cpu::ApplyKernel(kernel_sobel_horizontal, bw);
	auto v_res = cpu::ApplyKernel(kernel_sobel_vertical,   bw);

    double total_sum = 0;
	for (int i = 0; i < image.GetWidth() * image.GetHeight(); i++)
	{
		const uint8_t* h_pixel = h_res.GetColor(i);
		const uint8_t* v_pixel = v_res.GetColor(i);
		total_sum += std::sqrt(h_pixel[0] * h_pixel[0] + v_pixel[0] * v_pixel[0]);
    }

	float res = (float)total_sum / (image.GetWidth() * image.GetHeight());
	LOG_INFO("Total sum {} \n rate {}", total_sum, res);
	return res;
}


struct MainWindow : Module
{
	Ref<Texture2D> mImage;
	Ref<std::vector<Pixel>> mPixels;
	Ref<SelectibleImageWindow> mSelectibleImageWindow;
	Ref<gpu::MeanShift> mMeanShift = CreateRef<gpu::MeanShift>();

	MainWindow()
	{
		mWindowFlags |= ImGuiWindowFlags_NoCollapse;
	}

	void Draw(DeltaTime dt) override
	{
        ImGui::Begin("Main window", NULL, mWindowFlags);
        {
			ImGui::BeginChild("Steps", ImVec2(200, 200));

			if (ImGui::Button("Stage 1: Choose image", { 200, 50 }))
			{
				try {
					std::string image_path = OpenFileDialog("jpg,png");
					cpu::Image image(image_path);
					RateDitailsOnImage(image);
					mImage  = CreateRef<Texture2D>(Renderer::ResizeTexture2D(image, 400, 200));
					mPixels = CreateRef<std::vector<Pixel>>(GetPixels(*mImage));
					mSelectibleImageWindow = CreateRef<SelectibleImageWindow>(mImage);

                    LOG_INFO("{} {}", mImage->GetWidth(), mImage->GetHeight());
				}
				catch (std::exception& e) {
					LOG_WARN(e.what());
				}
			}
			
			 if (mImage)
			 {
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				if (ImGui::Button("Stage 2: Select area", { 200, 50 }))
				{
					mSelectibleImageWindow->mIsOpen = true;
					UI::AddModule(mSelectibleImageWindow);
				}
			
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				if (ImGui::Button("Stage 3: Run", { 200, 50 }))
				{
					float  radius = mSelectibleImageWindow->mRadius * mImage->GetWidth();
					ImVec2 center = mSelectibleImageWindow->mCircleCenter;
					center.x = floor(center.x * mImage->GetWidth());
					center.y = floor(center.y * mImage->GetHeight());
			
					std::vector<MeanShiftBreed> population;
					for (int i = 0; i < 5; i++)
						population.push_back(MeanShiftBreed(mPixels, mMeanShift, center, radius));

					auto result = GeneticAlgorithm(population, 10);
			
					// Print result
					std::vector<std::vector<Pixel>> snapshots;
					MeanShitParams params = ExtractParams(result.back()[0].mGens);

					//LOG_INFO("Params:\n radius: {}\n distance: {}\n color: {}\n brightness: {} \n\n",
					//			params.Radius, params.DistanceCoef, params.ColorCoef, params.BrightnessCoef);

					for (const auto& entity : result.back())
					{
						MeanShitParams params = ExtractParams(entity.mGens);

						LOG_INFO("\nParams:\n radius: {}\n distance: {}\n color: {}\n brightness: {} \n",
									params.Radius, params.DistanceCoef, params.ColorCoef, params.BrightnessCoef);
					}
			
					auto clusters = mMeanShift->Run(*mPixels, params, &snapshots);
					
					// Draw snapshots
					UI::AddModule<ImageGralleryWindow>(GetRefImagesFromPixelData(snapshots, mImage->mSpecification));
					
					// Draw clusters
					UI::AddModule<ImageWindow>(GetImageFromClusters(clusters, mImage->mSpecification));
					
					int evaluation = CalculateTargetValue(clusters, center, radius);
					LOG_INFO("Evalueation: {}", evaluation);
				}
			 }
			ImGui::EndChild();

			if (mImage)
			{
				ImGui::SameLine(0.0f, 20.0f);
				ImGui::Image((ImTextureID)mImage->GetRendererID(), ImVec2(200, 200));
			}

			ImGui::Dummy(ImVec2(0, 20));
			static int iterations= 5;
            static int radius   = 140;
            static int distance = 5;
            static int color	= 15;
            static int brightness = 1;
            static Ref<ImageWindow> image_window;
			static Ref<ImageWindow> image_window_selected_area;

            if (!image_window && mImage)
            {
                image_window = CreateRef<ImageWindow>(mImage, "segments");
                UI::AddModule(image_window);

				image_window_selected_area = CreateRef<ImageWindow>(mImage, "selected");
                UI::AddModule(image_window_selected_area);
            }

            bool changed = false;
			changed |= ImGui::SliderInt("iterations", &iterations, 1, 20);
            changed |= ImGui::SliderInt("Radius",     &radius,     50, 300);
            changed |= ImGui::SliderInt("distance",   &distance,   1,  100);
            changed |= ImGui::SliderInt("color",      &color,      1,  100);
            changed |= ImGui::SliderInt("brightness", &brightness, 1,  100);

            if (changed)
            {
                MeanShitParams params;
				params.Iterations = iterations;
                params.Radius = radius;
                params.DistanceCoef = distance;
                params.ColorCoef = color;
                params.BrightnessCoef = brightness;

                auto clusters = mMeanShift->Run(*mPixels, params);
                image_window->mImage = CreateRef<gpu::Image>(GetImageFromClusters(clusters, mImage->mSpecification));

				float x = floor(mSelectibleImageWindow->mPointInClass.x * mImage->GetWidth());
				float y = floor(mSelectibleImageWindow->mPointInClass.y * mImage->GetHeight());

				auto cluster_id = GetClusterByPixel(clusters, ImVec2(x, y));
				image_window_selected_area->mImage =
					CreateRef<gpu::Image>(GetImageFromCluster(clusters[cluster_id], mImage->mSpecification));
            }

		}
		ImGui::End();
	}

};