#pragma once

#include "core.h"
#include "opencl_base.h"

namespace Bubble
{
	enum class DeviceType { CPU, GPU, ALL };

	inline std::vector<cl::Device> GetDevices(const cl::Platform& platform, DeviceType device_type)
	{
		cl_int clDeviceType;
		switch (device_type)
		{
			case DeviceType::CPU:
				clDeviceType = CL_DEVICE_TYPE_CPU;
				break;
			case DeviceType::GPU:
				clDeviceType = CL_DEVICE_TYPE_GPU;
				break;
			case DeviceType::ALL:
				clDeviceType = CL_DEVICE_TYPE_ALL;
				break;
			default:
				BUBBLE_CORE_ASSERT(false, "Invalid device type");
		}

		std::vector<cl::Device> devices;
		platform.getDevices(clDeviceType, &devices);
		return devices;
	}

}