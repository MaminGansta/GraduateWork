
#include "module.h"

namespace Bubble
{
	std::atomic<int> Module::sGlobalID = 0;

	Module::Module()
		: mID(sGlobalID++),
		  mIsOpen(true),
		  mWindowFlags(0)
	{}

}