
#include "auto_contrast_cpu.h"

cpu::Image cpu::AutoContrast(const Image& image)
{
    Image res(Texture2DSpecification{ image.GetWidth(), image.GetHeight() });
    Histogram hist(image);

    // cut 1% from bounds
    int cut_amount = 0.1f * image.GetHeight() * image.GetWidth();
    int x_min, x_max;

    // find new bounds
    int count = 0;
    for (int i = 0; i < 256; i++)
    {
        count += hist.cbrightness[i];
        if (count >= cut_amount)
        {
            x_min = i;
            break;
        }
    }

    count = 0;
    for (int i = 255; i >= 0; i--)
    {
        count += hist.cbrightness[i];
        if (count >= cut_amount)
        {
            x_max = i;
            break;
        }
    }

    float c = 255.0f / (x_max - x_min);
    for (int i = 0; i < image.GetHeight() * image.GetWidth(); i++)
    {
        const uint8_t* pixel = image.GetColor(i);
        auto chanels = image.GetChannels();

        float Y = 0.2126f * pixel[0] + 0.7152f * pixel[1] + 0.0722 * pixel[2];
        float U = -0.0999f * pixel[0] - 0.336f * pixel[1] + 0.436f * pixel[2];
        float V = 0.615f * pixel[0] - 0.5586f * pixel[1] - 0.0563 * pixel[2];

        // Correction
        Y = (Y - float(x_min) / 255.0f) * c;

        uint8_t temp[4];
        temp[0] = std::clamp<int>(Y + 1.2803f * V, 0, 255);
        temp[1] = std::clamp<int>(Y - 0.2148f * U - 0.4805f * V, 0, 255);
        temp[2] = std::clamp<int>(Y + 2.1279f * U, 0, 255);
        res.SetColor(temp, i);
    }

    return res;
}
