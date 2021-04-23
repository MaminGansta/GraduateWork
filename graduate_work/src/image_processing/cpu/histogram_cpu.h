#pragma once
#include "image_cpu.h"

namespace cpu
{

struct Histogram
{
    float brightness[256];
    float red[256];
    float green[256];
    float blue[256];

    uint32_t cbrightness[256];
    uint32_t cred[256];
    uint32_t cgreen[256];
    uint32_t cblue[256];

    Histogram();

    Histogram(const Image& image)
    {
        CalculateHistogram(image);
    }

    void CalculateHistogram(const Image& image);
};

}