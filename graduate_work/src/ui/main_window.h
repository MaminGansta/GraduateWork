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
	Ref<ImageGalleryWindow> mImageGalleryWindow;
	Ref<gpu::MeanShift> mMeanShift = CreateRef<gpu::MeanShift>();
	MeanShitParams mStaticMSParams{ 5, 140, 1, 1, 1 };

	MainWindow()
	{
		mWindowFlags |= ImGuiWindowFlags_NoCollapse;
	}

	void Draw(DeltaTime dt) override
	{
		bool ms_params_changed = false;

        ImGui::Begin("Main window", NULL, mWindowFlags);
        {
			ImGui::BeginChild("Steps", ImVec2(300, 200));

			if (ImGui::Button("Step 1: Choose image", { 200, 50 }))
			{
				try {
					std::string image_path = OpenFileDialog("jpg,png");
					cpu::Image image(image_path);
					RateDitailsOnImage(image);
					mImage  = CreateRef<Texture2D>(Renderer::ResizeTexture2D(image, 200, 200));
					mPixels = CreateRef<std::vector<Pixel>>(GetPixels(*mImage));
					mSelectibleImageWindow = CreateRef<SelectibleImageWindow>(mImage);
					mImageGalleryWindow	   = CreateRef<ImageGalleryWindow>();
					UI::AddModule(mImageGalleryWindow);
				}
				catch (std::exception& e) {
					LOG_WARN(e.what());
				}
			}
			
			 if (mImage)
			 {
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				if (ImGui::Button("Step 2: Select area", { 200, 50 }))
				{
					mSelectibleImageWindow->mIsOpen = true;
					UI::AddModule(mSelectibleImageWindow);
				}
			
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				if (ImGui::Button("Step 3: Run", { 200, 50 }))
				{
					auto [class_point, circle_center, circle_radius] = ExtractOutliningParams();

					std::vector<MeanShiftBreed> population;
					for (int i = 0; i < 5; i++)
						population.push_back(MeanShiftBreed(mPixels, mMeanShift.get(), class_point, circle_center, circle_radius));

					auto result = GeneticAlgorithm(population, 10);
			
					// Interpret result 
					int   id = -1;
					int   max_id = 0;
					float max_target_value = -1e5;

					for (auto& entity : result.back())
					{
						id++;
						float target_value = entity.GetTargetValue();
						MeanShitParams params = entity.ExtractParams();

						if (max_target_value < target_value)
						{
							max_target_value = target_value;
							max_id = id;
						}

						LOG_INFO("Target: {} Params: radius: {} distance: {} color: {} brightness: {}",
									target_value, params.Radius, params.DistanceCoef, params.ColorCoef, params.BrightnessCoef);
					}

					ms_params_changed = true;
					mStaticMSParams = result.back()[max_id].ExtractParams();

					std::vector<std::vector<Pixel>> snapshots;
					auto clusters = mMeanShift->Run(*mPixels, mStaticMSParams, &snapshots);
					mImageGalleryWindow->SetImages(GetRefImagesFromPixelData(snapshots, mImage->mSpecification));
				}
			 }
			ImGui::EndChild();

			if (mImage)
			{
				ImGui::SameLine(0.0f, 20.0f);
				ImGui::Image((ImTextureID)mImage->GetRendererID(), ImVec2(200, 200));
			}

			ImGui::Dummy(ImVec2(0, 20));
			static Ref<ImageWindow> image_window;
			static Ref<ImageWindow> image_window_selected_area;

			if (!image_window && mImage)
			{
			    image_window = CreateRef<ImageWindow>(mImage, "segments");
			    UI::AddModule(image_window);

				 image_window_selected_area = CreateRef<ImageWindow>(mImage, "selected");
			    UI::AddModule(image_window_selected_area);
			}

			if (mImage)
			{
				ms_params_changed |= ImGui::SliderInt("iterations", &mStaticMSParams.Iterations, 1, 20);
				ms_params_changed |= ImGui::SliderInt("Radius", &mStaticMSParams.Radius, 20, 300);
				ms_params_changed |= ImGui::SliderInt("distance", &mStaticMSParams.DistanceCoef, 1, 100);
				ms_params_changed |= ImGui::SliderInt("color", &mStaticMSParams.ColorCoef, 1, 100);
				ms_params_changed |= ImGui::SliderInt("brightness", &mStaticMSParams.BrightnessCoef, 1, 100);

				if (ImGui::Button("Apply", ImVec2(50, 30)))
				{
					ms_params_changed = true;
				}
			}

			if (ms_params_changed)
			{
				std::vector<std::vector<Pixel>> snapshots;
				auto clusters = mMeanShift->Run(*mPixels, mStaticMSParams, &snapshots);
				mImageGalleryWindow->SetImages(GetRefImagesFromPixelData(snapshots, mImage->mSpecification));
				
			    image_window->mImage = CreateRef<gpu::Image>(GetImageFromClusters(clusters, mImage->mSpecification));
				
				auto [class_point, circle_center, circle_radius] = ExtractOutliningParams();
				
				// Draw selected cluster
				auto cluster_id = GetClusterByPixel(clusters, class_point);
				image_window_selected_area->mImage =
					CreateRef<gpu::Image>(GetImageFromCluster(clusters[cluster_id], mImage->mSpecification));
				
				int target = CalculateTargetValue(clusters, class_point, circle_center, circle_radius);
				
				LOG_INFO("target: {}  params: r {} d {} c {} b {} i {}",
					target, mStaticMSParams.Radius, mStaticMSParams.DistanceCoef,mStaticMSParams.ColorCoef,
					mStaticMSParams.BrightnessCoef, mStaticMSParams.Iterations);

                //auto clusters = cpu::MeanShiftPP::Run(*mPixels, mStaticMSParams);
                //image_window->mImage = CreateRef<gpu::Image>(GetImageFromClusters(clusters, mImage->mSpecification));
			}

		}
		ImGui::End();
	}

	std::tuple<ImVec2, ImVec2, float> ExtractOutliningParams()
	{
		float  circle_radius = mSelectibleImageWindow->mRadius * mImage->GetWidth();
		ImVec2 circle_center = mSelectibleImageWindow->mCircleCenter;
		circle_center.x = floor(circle_center.x * mImage->GetWidth());
		circle_center.y = floor(circle_center.y * mImage->GetHeight());

		ImVec2 class_point = mSelectibleImageWindow->mPointInClass;
		class_point.x = floor(class_point.x * mImage->GetWidth());
		class_point.y = floor(class_point.y * mImage->GetHeight());

		return { class_point, circle_center, circle_radius };
	}

};