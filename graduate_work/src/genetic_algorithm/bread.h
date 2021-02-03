#pragma once

#include "bubble.h"
#include "tools.h"
#include "RNG.h"

#include "cpu/image/image_cpu.h"
#include "gpu/meanshift/meanshift_gpu.h"

#include <bitset>


#define MEANSHIFT_ITERATIONS 16

static MeanShitParams ExtractParams(const std::bitset<32>& bits)
{
    MeanShitParams params;
    uint8_t data[4];
    for (int i = 0; i < 4; i++)
    {
        data[i] = *((uint8_t*)&bits + i);
    }
    params.Radius         = data[0];
    params.DistanceCoef   = data[1] & 0b00011111; // Max value of coefficient is 31
    params.ColorCoef      = data[2] & 0b00011111;
    params.BrightnessCoef = data[3] & 0b00011111;
    params.Iterations     = MEANSHIFT_ITERATIONS;
    return params;
}


struct MeanShiftBreed
{
    Ref<cpu::Image> mImage;
    Ref<gpu::MeanShift> mMeanShift;
    Ref<RNG> mRng;
    ImVec2 mCenter;
    float mRadius;
    std::bitset<32> mGens;

    MeanShiftBreed(const Ref<cpu::Image>& image, 
                   const Ref<gpu::MeanShift>& meanshift,
                   ImVec2 center,
                   float radius)
        : mImage(image),
          mMeanShift(meanshift),
          mCenter(center),
          mRadius(radius)
    {
        mRng = CreateRef<RNG>();
        for (int i = 0; i < mGens.size(); i++)
        {
            mGens.set(mRng->Get<int>(0, 1));
        }
    }

    inline float GetTargetValue(MeanShiftBreed entity)
    {
        auto params = ExtractParams(mGens);
        auto clusters = mMeanShift->Run(*mImage.get(), params);
        return MeanshiftEvaluation(clusters, mCenter, mRadius);
    }

    inline MeanShiftBreed Crossover(MeanShiftBreed& other)
    {
        MeanShiftBreed child = other;
        int breakpoint = mRng->Get<int>(0, mGens.size());

        MeanShiftBreed* parents[2] = { this, &other };
        int left = mRng->Get<int>(0, 1);
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
        for (int i = 0; i < mGens.size(); i++)
        {
            if (mRng->Get<float>(0.0f, 1.0f) < 0.1f)
            {
                mGens.flip(i);
            }
        }
    }

};