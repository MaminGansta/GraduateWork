
#include "module.h"

namespace Bubble
{
	int Module::sGlobalID = 0;

	Module::Module()
		: mID(sGlobalID++),
		  mIsOpen(true)
	{}

}