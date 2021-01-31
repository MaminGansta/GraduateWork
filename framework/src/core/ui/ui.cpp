
#include "ui.h"


namespace Bubble
{
	std::vector<Ref<Module>> UI::sModules;
	std::function<void()> UI::sDrawMenuBar;

	int UI::AddModule(Ref<Module>&& ui_module)
	{
		sModules.push_back(std::move(ui_module));
		return sModules.back()->GetID();
	}

	void UI::RemoveModule(int module_id)
	{
		auto iterator = std::find_if(sModules.begin(), sModules.end(),
			[module_id](const auto& ui_module) { return ui_module->GetID() == module_id; });
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
		if (sDrawMenuBar)
		{
			sDrawMenuBar();
		}
		mImGuiControll.EndMenuBar();

		for (int i = 0; i < sModules.size(); i++)
		{
			auto& ui_module = sModules[i];
			if (!ui_module->IsOpen())
			{
				auto iterator = std::find(sModules.begin(), sModules.end(), ui_module);
				sModules.erase(iterator);
			}
		}

		for (auto& ui_module : sModules)
		{
			ui_module->Draw(dt);
		}

		mImGuiControll.End();
	}

}
