#pragma once

#include "time/timer.h"
#include "module/module.h"
#include "imgui_controll/imgui_controll.h"

#include <ranges>
#include <algorithm>
#include <functional>


namespace Bubble
{
	struct UI
	{
		static std::vector<Ref<Module>> sModules;
		static std::function<void()> sDrawMenuBar;
		ImGuiControll mImGuiControll;

		UI() = default;
		UI(Window* window);
		~UI();

		void OnUpdate(DeltaTime dt);

		template <typename T, typename ...Args>
		static int AddModule(Args&& ...args);
		static int AddModule(const Ref<Module>& ui_module);
		static void RemoveModule(int module_id);
		static void SetMenuBar(std::function<void()>&& draw_menubar);
	};

	template <typename T, typename ...Args>
	int Bubble::UI::AddModule(Args&& ...args)
	{
		sModules.push_back(CreateRef<T>(std::forward<Args>(args)...));
		return sModules.back()->GetID();
	}

}