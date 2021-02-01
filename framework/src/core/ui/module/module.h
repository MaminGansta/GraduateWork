#pragma once

#include "core.h"
#include <atomic>

namespace Bubble
{
	struct Module
	{
	private:
		static std::atomic<int> sGlobalID;

	public:
		int mWindowFlags;
		bool mIsOpen;
		int mID;

		Module();
		bool IsOpen() { return mIsOpen; }
		int GetID() { return mID; }

		virtual void Draw(DeltaTime) = 0;
		virtual ~Module() {};
	};
}