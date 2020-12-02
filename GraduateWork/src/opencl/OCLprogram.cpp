

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include "CL/cl.hpp"
#include <fstream>


std::tuple<cl::Program, cl::Context, cl::Device, cl_int> CreateProgram(const std::string& name, const char* vendor)
{
	std::vector<cl::Platform> platforms;

	cl::Platform::get(&platforms);
	_ASSERT(platforms.size() > 0);

	auto platform = platforms.front();

	// get all divices
	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
	_ASSERT(devices.size() > 0);

	// find device by vendor name
	cl::Device device;
	for (int i = 0; i < devices.size(); i++)
	{
		const cl::STRING_CLASS& vndr = devices[i].getInfo<CL_DEVICE_VENDOR>();
		
		if (vndr._Equal(vendor))
			device = devices[i];
	}

	// if vendor was not found
	if (!device.getInfo<CL_DEVICE_AVAILABLE>())
		return { cl::Program(), cl::Context(), cl::Device(), -200 };

	// load file
	std::ifstream ProgramFile(name);
	if (!ProgramFile.is_open()) 
		return { cl::Program(), cl::Context(), cl::Device(), -100};

	// make source string 
	std::string src(std::istreambuf_iterator<char>(ProgramFile), (std::istreambuf_iterator<char>()));
	
	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));


	cl::Context context(device);
	cl::Program program(context, sources);

	// build
	auto error = program.build("-cl-std=CL1.2");

	cl_build_status status = program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device);
	if (status == CL_BUILD_ERROR)
	{
		// Get the build log
		std::string name = device.getInfo<CL_DEVICE_NAME>();
		std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
		printf("Build log for %s :\n %s", name.c_str(), buildlog.c_str());
	}

	return { program, context, device, error };
}