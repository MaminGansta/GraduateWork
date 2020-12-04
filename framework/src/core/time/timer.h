#pragma once
#include <iostream>
#include <chrono>

#include "time.h"


namespace Bubble
{
	using namespace std::chrono;

	class Timer
	{
		static high_resolution_clock::time_point sProgramStart;
		high_resolution_clock::time_point mLastTime = Now();
		DeltaTime mDeltatime = 0.0f;

	public:
		DeltaTime Update();
		DeltaTime GetDeltaTime();

		// Time from the program start
		static Time GetTime();

	private:
		static high_resolution_clock::time_point Now();
	};

}
