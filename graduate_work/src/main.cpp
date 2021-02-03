
#include "bubble_entry_point.h"

#include "gen_alg.h"
#include "main_window.h"


struct MyApplication : Application
{
	Ref<SelectibleImageWindow> selectible_window;

	MyApplication()
		: Application("Image segmentation")
	{}

	void OnCreate()
	{
		UI::AddModule<MainWindow>();
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