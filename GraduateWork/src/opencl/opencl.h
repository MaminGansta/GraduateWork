#pragma once

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include "CL/cl.hpp"
#include <cstdio>

std::tuple<cl::Program, cl::Context, cl::Device, cl_int> CreateProgram(const std::string& name, const char* vendor);
const char* getErrorString(cl_int error);