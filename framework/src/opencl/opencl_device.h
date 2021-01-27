#pragma once

#include "core.h"
#include "opencl_base.h"

namespace Bubble
{
	enum class DeviceType { CPU, GPU, ALL };
	inline const char* GetDeviceTypeString(DeviceType i) { const char* names[] = { "CPU", "GPU", "ALL" }; return names[(int)i]; }

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
		cl_int status;
		std::vector<cl::Device> devices;
		status = platform.getDevices(clDeviceType, &devices);
		DEBUG_LOG_WARN("[OpenCL Debug Log]\n Platform: {} \n Device type: {} \n Device status: {}",
			platform.getInfo<CL_PLATFORM_NAME>(), GetDeviceTypeString(device_type), clGetErrorString(status));
		return devices;
	}


	inline std::vector<cl::Device> GetAllDevices(DeviceType device_type)
	{
		std::vector<cl::Platform> platforms;
		std::vector<cl::Device> devices;
		cl::Platform::get(&platforms);

		for (auto& platform : platforms)
		{
			auto platform_devices = GetDevices(platform, device_type);
			for (auto& device : platform_devices)
			{
				devices.push_back(device);
			}
		}

		return devices;
	}

}