#pragma once

#include "cl/cl.hpp"

namespace Bubble
{
	inline std::string clGetErrorString(cl_int error)
	{
		switch (error)
		{
			// run-time and JIT compiler errors
			case 0: return "CL_SUCCESS\r\n";
			case -1: return "CL_DEVICE_NOT_FOUND\r\n";
			case -2: return "CL_DEVICE_NOT_AVAILABLE\r\n";
			case -3: return "CL_COMPILER_NOT_AVAILABLE\r\n";
			case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE\r\n";
			case -5: return "CL_OUT_OF_RESOURCES\r\n";
			case -6: return "CL_OUT_OF_HOST_MEMORY\r\n";
			case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE\r\n";
			case -8: return "CL_MEM_COPY_OVERLAP\r\n";
			case -9: return "CL_IMAGE_FORMAT_MISMATCH\r\n";
			case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED\r\n";
			case -11: return "CL_BUILD_PROGRAM_FAILURE\r\n";
			case -12: return "CL_MAP_FAILURE\r\n";
			case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET\r\n";
			case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST\r\n";
			case -15: return "CL_COMPILE_PROGRAM_FAILURE\r\n";
			case -16: return "CL_LINKER_NOT_AVAILABLE\r\n";
			case -17: return "CL_LINK_PROGRAM_FAILURE\r\n";
			case -18: return "CL_DEVICE_PARTITION_FAILED\r\n";
			case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE\r\n";

			// compile-time errors
			case -30: return "CL_INVALID_VALUE\r\n";
			case -31: return "CL_INVALID_DEVICE_TYPE\r\n";
			case -32: return "CL_INVALID_PLATFORM\r\n";
			case -33: return "CL_INVALID_DEVICE\r\n";
			case -34: return "CL_INVALID_CONTEXT\r\n";
			case -35: return "CL_INVALID_QUEUE_PROPERTIES\r\n";
			case -36: return "CL_INVALID_COMMAND_QUEUE\r\n";
			case -37: return "CL_INVALID_HOST_PTR\r\n";
			case -38: return "CL_INVALID_MEM_OBJECT\r\n";
			case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR\r\n";
			case -40: return "CL_INVALID_IMAGE_SIZE\r\n";
			case -41: return "CL_INVALID_SAMPLER\r\n";
			case -42: return "CL_INVALID_BINARY\r\n";
			case -43: return "CL_INVALID_BUILD_OPTIONS\r\n";
			case -44: return "CL_INVALID_PROGRAM\r\n";
			case -45: return "CL_INVALID_PROGRAM_EXECUTABLE\r\n";
			case -46: return "CL_INVALID_KERNEL_NAME\r\n";
			case -47: return "CL_INVALID_KERNEL_DEFINITION\r\n";
			case -48: return "CL_INVALID_KERNEL\r\n";
			case -49: return "CL_INVALID_ARG_INDEX\r\n";
			case -50: return "CL_INVALID_ARG_VALUE\r\n";
			case -51: return "CL_INVALID_ARG_SIZE\r\n";
			case -52: return "CL_INVALID_KERNEL_ARGS\r\n";
			case -53: return "CL_INVALID_WORK_DIMENSION\r\n";
			case -54: return "CL_INVALID_WORK_GROUP_SIZE\r\n";
			case -55: return "CL_INVALID_WORK_ITEM_SIZE\r\n";
			case -56: return "CL_INVALID_GLOBAL_OFFSET\r\n";
			case -57: return "CL_INVALID_EVENT_WAIT_LIST\r\n";
			case -58: return "CL_INVALID_EVENT\r\n";
			case -59: return "CL_INVALID_OPERATION\r\n";
			case -60: return "CL_INVALID_GL_OBJECT\r\n";
			case -61: return "CL_INVALID_BUFFER_SIZE\r\n";
			case -62: return "CL_INVALID_MIP_LEVEL\r\n";
			case -63: return "CL_INVALID_GLOBAL_WORK_SIZE\r\n";
			case -64: return "CL_INVALID_PROPERTY\r\n";
			case -65: return "CL_INVALID_IMAGE_DESCRIPTOR\r\n";
			case -66: return "CL_INVALID_COMPILER_OPTIONS\r\n";
			case -67: return "CL_INVALID_LINKER_OPTIONS\r\n";
			case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT\r\n";

			// extension errors
			case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR\r\n";
			case -1001: return "CL_PLATFORM_NOT_FOUND_KHR\r\n";
			case -1002: return "CL_INVALID_D3D10_DEVICE_KHR\r\n";
			case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR\r\n";
			case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR\r\n";
			case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR\r\n";

			default: return "Unknown OpenCL error\r\n";
		}
	}

}