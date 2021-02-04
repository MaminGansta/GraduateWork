#pragma once

#include "bubble.h"
#include "tools.h"
#include "cpu/meanshift_cpu.h"

struct MeanShitParams
{
	int Iterations = 0;
	int Radius = 0;
	int DistanceCoef = 0;
	int ColorCoef = 0;
	int BrightnessCoef = 0;
};

namespace gpu
{
	
	struct MeanShift
	{
		cl::Kernel mKernel;
		cl::Context mContext;
		cl::Device mDevice;

		uint32_t active_buffer_id = 0;
		uint32_t sub_buffer_id = 1;
		cl::Buffer mBuffers[2];

		cl::CommandQueue mQueue;


		inline MeanShift()
		{
			auto kernel_name = "MeanShift";
            auto kernel_path = "resources/kernels/meanshift.cl";
			auto [kernel, context, device] = CreateKernel(kernel_path, kernel_name, DeviceType::GPU);
			mKernel = kernel;
			mContext = context;
			mDevice = device;
			mQueue = cl::CommandQueue(mContext, mDevice);
		}

		inline std::vector<Cluster<Pixel>> Run(std::vector<Pixel>& pixels,
											   MeanShitParams params, 
											   std::vector<std::vector<Pixel>>* snapshots = nullptr)
		{
			std::vector<Pixel> shifted(pixels.size());
			if (snapshots)
			{
				snapshots->push_back(pixels);
			}

			cl_int err = 0;
			mBuffers[0] = cl::Buffer(mContext, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Pixel) * pixels.size(), pixels.data(), &err);
			mBuffers[1] = cl::Buffer(mContext, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY, sizeof(Pixel) * pixels.size(), nullptr, &err);

			for (int i = 0; i < params.Iterations; i++)
			{
				clcall(mKernel.setArg(0, mBuffers[active_buffer_id]));
				clcall(mKernel.setArg(1, mBuffers[sub_buffer_id]));
				clcall(mKernel.setArg(2, (int)pixels.size()));
				clcall(mKernel.setArg(3, params.Radius));
				clcall(mKernel.setArg(4, params.DistanceCoef));
				clcall(mKernel.setArg(5, params.ColorCoef));
				clcall(mKernel.setArg(6, params.BrightnessCoef));

				int parts = ceil((float)pixels.size() / 400'000);
				int step_size = pixels.size() / parts;

				for (int j = 0; j < parts; j++)
				{
					clcall(mQueue.enqueueNDRangeKernel(mKernel, cl::NDRange(step_size * j), cl::NDRange(pixels.size() / parts)));
				}
				cl::finish();
				std::swap(active_buffer_id, sub_buffer_id);

				if (snapshots)
				{
					clcall(mQueue.enqueueReadBuffer(mBuffers[active_buffer_id], GL_TRUE, 0, sizeof(Pixel) * pixels.size(), shifted.data()));
					snapshots->push_back(shifted);
				}
			}

			clcall(mQueue.enqueueReadBuffer(mBuffers[active_buffer_id], GL_TRUE, 0, sizeof(Pixel) * pixels.size(), shifted.data()));
			return cpu::MeanShift::CreateClusters(pixels, shifted, params.Radius);
		}

	};
}