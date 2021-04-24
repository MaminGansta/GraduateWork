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
					cpu::Image image(image_path);
					RateDitailsOnImage(image);
					mImage = CreateRef<Texture2D>(Renderer::ResizeTexture2D(image, 400, 300));
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
					Ref<gpu::MeanShift> meanshift  = CreateRef<gpu::MeanShift>();
					Ref<std::vector<Pixel>> pixels = CreateRef<std::vector<Pixel>>(GetPixels(*mImage));
			
					float  radius = mSelectibleImageWindow->mRadius * mImage->GetWidth();
					ImVec2 center = mSelectibleImageWindow->mCircleCenter;
					center.x = floor(center.x * mImage->GetWidth());
					center.y = floor(center.y * mImage->GetHeight());
			
					std::vector<MeanShiftBreed> population(5, MeanShiftBreed(pixels, meanshift, center, radius));
					auto result = GeneticAlgorithm(population, 10);
			
					// Print result
					std::vector<std::vector<Pixel>> snapshots;
					MeanShitParams params = ExtractParams(result.back()[0].mGens);

					//LOG_INFO("Params:\n radius: {}\n distance: {}\n color: {}\n brightness: {} \n\n",
					//			params.Radius, params.DistanceCoef, params.ColorCoef, params.BrightnessCoef);

					for (const auto& entity : result.back())
					{
						MeanShitParams params = ExtractParams(entity.mGens);

						LOG_INFO("Params:\n radius: {}\n distance: {}\n color: {}\n brightness: {} \n\n",
									params.Radius, params.DistanceCoef, params.ColorCoef, params.BrightnessCoef);
					}
			
					auto clusters = meanshift->Run(*pixels, params, &snapshots);
					
					// Draw snapshots
					UI::AddModule<ImageGralleryWindow>(GetRefImagesFromPixelData(snapshots, mImage->mSpecification));
					
					// Draw clusters
					UI::AddModule<ImageWindow>(GetImageFromClusters(clusters, mImage->mSpecification));
					
					int evaluation = MeanshiftEvaluation(clusters, center, radius);
					LOG_INFO("Evalueation: {}", evaluation);
				}
			 }

            static int radius = 100;
            static int distance = 25;
            static int color = 5;
            static int brightness = 1;
            static Ref<ImageWindow> image_window;
			static Ref<ImageWindow> image_window_selected_area;

            if (!image_window && mImage)
            {
                image_window = CreateRef<ImageWindow>(mImage, "test");
                UI::AddModule(image_window);

				image_window_selected_area = CreateRef<ImageWindow>(mImage, "selected");
                UI::AddModule(image_window_selected_area);
            }

            bool changed = false;
            changed |= ImGui::SliderInt("Radius",     &radius,     50, 300);
            changed |= ImGui::SliderInt("distance",   &distance,   0,  100);
            changed |= ImGui::SliderInt("color",      &color,      0,  100);
            changed |= ImGui::SliderInt("brightness", &brightness, 0,  100);

            if (changed)
            {
                static Ref<gpu::MeanShift> meanshift  = CreateRef<gpu::MeanShift>();
                static Ref<std::vector<Pixel>> pixels = CreateRef<std::vector<Pixel>>(GetPixels(*mImage));

                MeanShitParams params;
                params.Radius = radius;
                params.DistanceCoef = distance;
                params.ColorCoef = color;
                params.BrightnessCoef = brightness;

                auto clusters = meanshift->Run(*pixels, params);
                image_window->mImage = CreateRef<gpu::Image>(GetImageFromClusters(clusters, mImage->mSpecification));

				float w = floor(mSelectibleImageWindow->mCircleCenter.x * mImage->GetWidth());
				float h = floor(mSelectibleImageWindow->mCircleCenter.y * mImage->GetHeight());

				auto cluster_id = GetClusterByPixel(clusters, Pixel{ w, h });
				image_window_selected_area->mImage =
					CreateRef<gpu::Image>(GetImageFromCluster(clusters[cluster_id], mImage->mSpecification));
            }

		}
		ImGui::End();
	}

};