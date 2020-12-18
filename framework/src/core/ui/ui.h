#pragma once

#include "imgui.h"
#include "time/timer.h"
#include "module/module.h"
#include "imgui_controll/imgui_controll.h"

#include <algorithm>
#include <functional>


namespace Bubble
{
	struct UI
	{
		static std::vector<Scope<Module>> sModules;
		static std::function<void()> sDrawMenuBar;
		ImGuiControll mImGuiControll;

		UI() = default;
		UI(Window* window);
		~UI();

		void OnUpdate(DeltaTime dt);

		template <typename T, typename ...Args>
		static int AddModule(Args&& ...args)
		{
			sModules.push_back(CreateScope<T>(std::forward<Args>(args)...));
			return sModules.back()->GetID();
		}
		static int AddModule(Scope<Module>&& module);
		static void RemoveModule(int module_id);
		static void SetMenuBar(std::function<void()>&& draw_menubar);
	};

}