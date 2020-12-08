
#include "ui.h"


namespace Bubble
{
	std::vector<Scope<Module>> UI::sModules;
	std::function<void()> UI::sDrawMenuBar;

	int UI::AddModule(Scope<Module>&& module)
	{
		sModules.push_back(std::move(module));
		return sModules.back()->GetID();
	}

	void UI::RemoveModule(int module_id)
	{
		auto iterator = std::find_if(sModules.begin(), sModules.end(),
			[module_id](const Scope<Module>& module) { return module->GetID() == module_id; });
		sModules.erase(iterator);
	}

	void UI::SetMenuBar(std::function<void()>&& draw_menubar)
	{
		sDrawMenuBar = draw_menubar;
	}


	UI::UI(Window* window)
		: mImGuiControll(window)
	{
		mImGuiControll.OnAttach();
	}

	UI::~UI()
	{
		mImGuiControll.OnDetach();
	}

	void UI::OnUpdate(DeltaTime dt)
	{
		mImGuiControll.Begin();

		mImGuiControll.BeginMenuBar();
		sDrawMenuBar ? sDrawMenuBar() : NULL;
		mImGuiControll.EndMenuBar();

		for (int i = 0; i < sModules.size(); i++)
		{
			auto& module = sModules[i];
			if (!module->IsOpen())
			{
				auto iterator = std::find(sModules.begin(), sModules.end(), module);
				sModules.erase(iterator);
			}
		}

		for (auto& module : sModules)
		{
			module->Draw(dt);
		}

		mImGuiControll.End();
	}

}
