#pragma once

#include "core.h"
#include "opencl_base.h"
#include "opencl_errors.h"
#include "opencl_device.h"

#include <stdexcept>
#include <fstream>

namespace Bubble
{
	inline std::tuple<cl::Kernel, cl::Context, cl::Device> 
				CreateKernel(const std::string& file_name, 
							 const std::string& kernel_name, 
							 const cl::Device& device)
	{
		std::ifstream kernel_file(file_name);

		if (!device.getInfo<CL_DEVICE_AVAILABLE>())
			throw std::runtime_error("Not valid OpenCL device");

		if (!kernel_file.is_open())
			throw std::runtime_error("No such file: " + file_name);

		std::string src(std::istreambuf_iterator<char>(kernel_file), (std::istreambuf_iterator<char>()));
		cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

		cl::Context context(device);
		cl::Program program(context, sources);

		auto error = program.build("-cl-std=CL1.2");
		cl_build_status status = program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device);

		if (status == CL_BUILD_ERROR)
		{
			std::string name = device.getInfo<CL_DEVICE_NAME>();
			std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
			LOG_CORE_ERROR("Build error log: \n File: {} Device: {} \n\nInfo:\n{}", file_name, name, buildlog);
			throw std::runtime_error("OpenCL kernel built failed: kernel name> " + kernel_name);
		}

		cl::Kernel kernel(program, kernel_name.c_str(), &error);

		if (error) {
			LOG_CORE_ERROR("Kernel build failed: {}", GetCLErrorString(error));
		}

		return { kernel, context, device };
	}


	inline std::tuple<cl::Kernel, cl::Context, cl::Device>
		CreateKernel(const std::string& file_name,
			const std::string& kernel_name,
			DeviceType device_type = DeviceType::ALL,
			int device_id = 0)
	{
		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		auto devices = GetDevices(platforms[0], device_type);

		device_id = std::min(device_id, (int)devices.size());
		return CreateKernel(file_name, kernel_name, devices[device_id]);
	}
}