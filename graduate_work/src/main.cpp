
#include "bubble_entry_point.h"

#include "gen_alg.h"
#include "main_window.h"

#include <bitset>


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
