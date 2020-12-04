#pragma once

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
	}

	return 0;
}


