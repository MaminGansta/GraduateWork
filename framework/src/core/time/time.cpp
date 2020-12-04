
#include "time.h"

namespace Bubble
{
	Time::Time(float time)
		: mTime(time)
	{}

	float Time::GetSeconds()
	{
		return mTime;
	}

	float Time::GetMilliseconds()
	{
		return mTime * 1000.0f;
	}

}