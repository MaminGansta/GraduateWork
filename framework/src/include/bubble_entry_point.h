#pragma once

#include "bubble.h"
#include "application/application.h"


// Implementation on client side
extern Bubble::Application* CreateApplication();

int main(int argc, char* argv[])
{
	try
	{
		Bubble::Scope<Bubble::Application> app(CreateApplication());
		app->Run();
	}
	catch (const std::exception& e) {
		LOG_CORE_ERROR("Exception has been trigered:\n {}", e.what());
		std::this_thread::sleep_for(5s);
	}

	return 0;
}


