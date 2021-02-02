
#include "bubble_entry_point.h"

#include "gen_alg.h"
#include "image_processing.h"
#include "gpu/meanshift/meanshift_gpu.h"

#include "main_window.h"
#include "image_window.h"
#include "selectible_image_window.h"
#include "image_gallerey.h"

struct MyApplication : Application
{
	Ref<SelectibleImageWindow> selectible_window;

	MyApplication()
		: Application("Image segmentation")
	{}

	void OnCreate()
	{
		void meanshift_test(const Ref<SelectibleImageWindow>&);
		void gen_alg_test();
		

		UI::AddModule<MainWindow>();

		//gen_alg_test();
		//meanshift_test(selectible_window);
	}

};

Application* CreateApplication()
{
	return new MyApplication;
}


// ====================== Test area ====================== 

struct MeanShiftBreed
{
	Ref<cpu::Image> mImage;
	Ref<RNG> mRNG = CreateRef<RNG>();
	uint32_t mGens[4] = {0};

	MeanShiftBreed(const Ref<cpu::Image>& image)
		: mImage(image)
	{}

	static inline float GetTargetValue(MeanShiftBreed entity)
	{

		return 0.0f;
	}

	static inline MeanShiftBreed Crossover(const MeanShiftBreed& a, const MeanShiftBreed& b)
	{ 
		return a;
	}

	static inline MeanShiftBreed Mutation (const MeanShiftBreed& a)
	{ 
		return a;
	}
};

//void gen_alg_test()
//{
//	std::vector<MeanShiftBreed> a(5);
//	GeneticAlgorithm(a, 10);
//}



// ====================== MeanShift ====================== 

void meanshift_test(const Ref<SelectibleImageWindow>& selectible_window)
{
	cpu::Image input("resources/images/car1.jpg");
	cpu::Image image(Renderer::ResizeTexture2D(input, 400, 320));

	

	// CPU
	//auto clusters = MeanShift::Run(pixels, 10, 3);
	
	MeanShitParams params;
	params.Radius = 131;
	params.DistanceCoef = 12;
	params.ColorCoef = 4;
	params.BrightnessCoef = 12;
	params.Iterations = 16;

	std::vector<std::vector<Pixel>> snapshots;

    // OpenCL
    gpu::MeanShift meanshift;
	std::vector<Cluster<Pixel>> clusters = meanshift.Run(image, params, &snapshots);
	
	// Draw snapshots
	UI::AddModule<ImageGralleryWindow>(GetRefImagesFromPixelData(snapshots, image.mSpecification));
	
	// Draw clusters
	UI::AddModule<ImageWindow>(GetImageFromClusters(clusters, image.mSpecification));
}