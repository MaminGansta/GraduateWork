#pragma once

#include "kernel.h"
#include "box_filter.h"
#include "gauss_filter.h"
#include "sharpness_filter.h"

#include "cpu/image/image_cpu.h"
#include "cpu/kmeans/kmeans_image_cpu.h"
#include "cpu/conv_calculation/conv_calculation_cpu.h"

#include "gpu/image/image_gpu.h"
#include "gpu/conv_calculation/conv_calculation_gpu.h"