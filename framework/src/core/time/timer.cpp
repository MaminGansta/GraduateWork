
#include "timer.h"


namespace Bubble
{
	high_resolution_clock::time_point Timer::sProgramStart = Now();

	DeltaTime Timer::Update()
	{
		high_resolution_clock::time_point now = Now();
		duration<float> time_dif = duration_cast<duration<float>>(now - mLastTime);
		mDeltatime = time_dif.count();
		mLastTime = now;
		return mDeltatime;
	}

	DeltaTime Timer::GetDeltaTime()
	{
		return mDeltatime;
	}

	std::chrono::high_resolution_clock::time_point Timer::Now()
	{
		return  high_resolution_clock::now();
	}

	Time Timer::GetTime()
	{
		high_resolution_clock::time_point now = Now();
		duration<float> time_dif = duration_cast<duration<float>>(now - sProgramStart);
		return time_dif.count();
	}

}