#pragma once

#include "core.h"


namespace Bubble
{
	struct Module
	{
	private:
		static int sGlobalID;

	public:
		bool mIsOpen;
		int mID;

		Module();
		bool IsOpen() { return mIsOpen; }
		int GetID() { return mID; }

		virtual void Draw(DeltaTime) = 0;
		virtual ~Module() {};
	};
}