#pragma once

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include "cl/cl.hpp"
#include "opencl_errors.h"
#include "log/log.h"

#if _DEBUG
#define clcall(func)\
		{\
			cl_int err = func;\
			if (err)\
			{\
				LOG_CORE_ERROR("[OpenCL Error]\n {}", clGetErrorString(err));\
				assert(false);\
			}\
		}
#else
#define clcall(func) (func);
#endif

namespace cl
{
	using string = STRING_CLASS;
}
