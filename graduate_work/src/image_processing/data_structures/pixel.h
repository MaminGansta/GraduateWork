#pragma once

#include <cstdint>
#include <cmath>

struct Pixel
{
    union
    {
        struct
        {
            float x, y;
            float color[4];
        };
        float vec[8] = { 0 };
    };

    Pixel& operator+= (const Pixel& other)
    {
        x += other.x;
        y += other.y;
        for (int channel = 0; channel < 3; channel++)
        {
            color[channel] += other.color[channel];
        }
        return *this;
    }

    Pixel& operator/= (float value)
    {
        x /= value;
        y /= value;
        for (int channel = 0; channel < 4; channel++)
        {
            color[channel] /= value;
        }
        return *this;
    }

    Pixel operator* (float val) const
    {
        Pixel pixel = *this;
        pixel.x *= val;
        pixel.y *= val;
        for (int channel = 0; channel < 3; channel++)
        {
            pixel.color[channel] *= val;
        }
        return pixel;
    }

    float Distance(const Pixel& other) const
    {
        float res = 0;
        float brightness1 = 0;
        float brightness2 = 0;

        res += std::pow(x - other.x, 2);
        res += std::pow(y - other.y, 2);
        for (int i = 0; i < 3; i++)
        {
            res += std::pow(color[i] - other.color[i], 2);
            brightness1 += color[i];
            brightness2 += other.color[i];
        }
        res += pow(brightness2 - brightness1, 2);

        return std::sqrt(res);
    }
};