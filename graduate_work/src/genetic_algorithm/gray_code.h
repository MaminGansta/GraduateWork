#pragma once

#include <cstdint>


inline uint32_t GrayEncode(unsigned int gray)
{
	return gray ^ (gray >> 1);
}


inline uint32_t GrayDecode(unsigned int gray)
{
	uint32_t bin = 0;
	while (gray != 0)
	{
		gray >>= 1;
		bin ^= gray;
	}
	return bin;
}
