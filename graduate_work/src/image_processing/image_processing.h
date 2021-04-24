#pragma once

#include "kernel.h"
#include "box_filter.h"
#include "gauss_filter.h"
#include "sharpness_filter.h"

#include "cpu/image_cpu.h"
#include "cpu/meanshift_cpu.h"
#include "cpu/conv_calculation_cpu.h"
#include "cpu/auto_contrast_cpu.h"
#include "cpu/histogram_alignment.h"
#include "cpu/YCbCr.h"

#include "gpu/image_gpu.h"
#include "gpu/meanshift_gpu.h"
#include "gpu/conv_calculation_gpu.h"

#include "data_structures/pixel.h"
#include "data_structures/cluster.h"