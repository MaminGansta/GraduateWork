
#include "opencl/opencl.h"
#include <fstream>
#include <time.h>

#define VENDOR_AMD "Advanced Micro Devices, Inc."


int main(void)
{
	auto [program, context, device, error] = CreateProgram("kernels/array_sum.cl", VENDOR_AMD);
	printf("compile time %0.4f \n", float(clock()) / CLOCKS_PER_SEC);

	if (error)
	{
		printf(getErrorString(error));
		return 1;
	}

	// 1 million additions
	std::vector<int> data(1024 * 1024 * 32, 1);

	// cpu
	volatile int cpu_sum = 0;
	time_t start = clock();

	for (int i = 0; i < data.size(); i+=4)
	{
		cpu_sum += data[i];
		cpu_sum += data[i+1];
		cpu_sum += data[i+2];
		cpu_sum += data[i+3];
	}

	printf("cpu - %f \n", float(clock() - start) / CLOCKS_PER_SEC);

	// gpu
	cl::Kernel kernel(program, "ArraySum", &error);

	int nValues_per_item = 64;
	uint32_t work_group_size = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device);

	uint32_t num_work_groups = data.size() / work_group_size;
	uint32_t NDRange = data.size() / nValues_per_item;

	cl::Buffer values_per_item(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int), &nValues_per_item, &error);
	cl::Buffer inBuffer(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * data.size(), data.data(), &error);
	cl::Buffer outBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int), nullptr, &error);

	error = kernel.setArg(0, values_per_item);
	error = kernel.setArg(1, inBuffer);
	error = kernel.setArg(2, outBuffer);


	start = clock();

	cl::CommandQueue queue(context, device);
	error = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(NDRange), cl::NDRange(work_group_size));

	int res = 0;
	error = queue.enqueueReadBuffer(outBuffer, GL_TRUE, 0, sizeof(int), &res);


	printf("gpu - %f \n", float(clock() - start) / CLOCKS_PER_SEC);

	return 0;
}