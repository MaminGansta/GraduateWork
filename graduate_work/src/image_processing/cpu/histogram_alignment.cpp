
#include "histogram_alignment.h"

namespace cpu
{

cpu::Image cpu::HistograsmAlignment(const Image& image)
{
    Image res(Texture2DSpecification{ image.GetWidth(), image.GetHeight() });
    Histogram hist(image);

    uint32_t cdf[256];
    uint32_t cdf_min;
    uint32_t count = 0;

    cdf[0] = hist.cbrightness[0];
    for (int i = 1; i < 256; i++)
    {
        cdf[i] = cdf[i - 1] + hist.cbrightness[i];
        count += hist.cbrightness[i];
    }

    for (int i = 0; i < 255; i++)
    {
        if (cdf[i] > 0)
        {
            cdf_min = cdf[i];
            break;
        }
    }

    for (int i = 0; i < image.GetHeight() * image.GetWidth(); i++)
    {
        const uint8_t* pixel = image.GetColor(i);

        float Y =  0.2126f * pixel[0] + 0.7152f * pixel[1] + 0.0722 * pixel[2];
        float U = -0.0999f * pixel[0] - 0.336f  * pixel[1] + 0.436f * pixel[2];
        float V =  0.615f  * pixel[0] - 0.5586f * pixel[1] - 0.0563 * pixel[2];

        // Correction
        Y = float(cdf[int(Y * 255.0f)] - cdf_min) / (count - 1);

        uint8_t temp[4];
        temp[0] = std::clamp<int>(Y + 1.2803f * V, 0, 255);
        temp[1] = std::clamp<int>(Y - 0.2148f * U - 0.4805f * V, 0, 255);
        temp[2] = std::clamp<int>(Y + 2.1279f * U, 0, 255);
        res[i] = temp;
    }

    return res;
}

}