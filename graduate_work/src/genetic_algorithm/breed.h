#pragma once

#include <bitset>

#include "bubble.h"
#include "tools.h"
#include "gray_code.h"
#include "gpu/meanshift_gpu.h"
#include "random.hpp"
using Random = effolkronium::random_static;


#define MEANSHIFT_ITERATIONS 20

inline MeanShitParams ExtractParams(const std::bitset<32>& bits)
{
    MeanShitParams params;
    uint8_t data[4];
    for (int i = 0; i < 4; i++)
    {
        data[i] = *((uint8_t*)&bits + i);
    }
    params.Radius         = data[0];
    params.DistanceCoef   = data[1];
    params.ColorCoef      = data[2];
    params.BrightnessCoef = data[3];
    params.Iterations     = MEANSHIFT_ITERATIONS;
    return params;
}


struct MeanShiftBreed
{
    Ref<std::vector<Pixel>> mPixels;
    Ref<gpu::MeanShift> mMeanShift;
    ImVec2 mCenter;
    float  mRadius;
    std::bitset<32> mGens;

    MeanShiftBreed(const Ref<std::vector<Pixel>>& pixels, 
                   const Ref<gpu::MeanShift>& meanshift,
                   ImVec2 center,
                   float  radius)
        : mPixels(pixels),
          mMeanShift(meanshift),
          mCenter(center),
          mRadius(radius)
    {
        for (int i = 0; i < mGens.size(); i++)
        {
            mGens.set(i, Random::get<int>(0, 1));
        }
        ClipValues();
    }

    inline float GetTargetValue()
    {
        auto params   = ExtractParams(mGens);
        auto clusters = mMeanShift->Run(*mPixels, params);
        return MeanshiftEvaluation(clusters, mCenter, mRadius);
    }

    inline MeanShiftBreed Crossover(MeanShiftBreed& other)
    {
        MeanShiftBreed child = other;
        int breakpoint = Random::get<int>(0, mGens.size() - 1);

        MeanShiftBreed* parents[2] = { this, &other };
        int left = Random::get<int>(0, 1);
        int right = (left + 1) % 2;

        for (int i = 0; i < mGens.size(); i++)
        {
            if (i < breakpoint)
            {
                child.mGens.set(i, parents[left]->mGens[i]);
            }
            else {
                child.mGens.set(i, parents[right]->mGens[i]);
            }
        }
        return child;
    }

    inline void Mutation()
    {
        // Mutation
        for (int i = 0; i < mGens.size(); i++)
        {
            if (Random::get<float>(0.0f, 1.0f) < 0.3f)
            {
                mGens.flip(i);
            }
        }
        ClipValues();
    }

    // Limit values in optimal range
    void ClipValues()
    {
        uint8_t* data[4];
        for (int i = 0; i < 4; i++)
        {
            data[i] = ((uint8_t*)&mGens + i);
        }
        *data[0] |= 0b01011000; // Min value of radius is 64
        *data[0] &= 0b01111111;
        *data[1] &= 0b00001111;
        *data[2] &= 0b00001111; // Max value of coefficient is 31
        *data[3] &= 0b00000011;
    }

};