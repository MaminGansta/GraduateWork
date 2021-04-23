
#include "histogram_cpu.h"

namespace cpu
{

void Histogram::CalculateHistogram(const Image& image)
{
    memset(cbrightness, 0, 256 * sizeof(uint32_t));
    memset(cred, 0, 256 * sizeof(uint32_t));
    memset(cgreen, 0, 256 * sizeof(uint32_t));
    memset(cblue, 0, 256 * sizeof(uint32_t));

    int size = image.GetWidth() * image.GetHeight();
    for (int i = 0; i < size; i++)
    {
        const uint8_t* color = image.GetColor(i);
        uint8_t br = (color[0] + color[1] + color[2]) / 3;
        cbrightness[br]++;
        cred[color[0]]++;
        cgreen[color[1]]++;
        cblue[color[2]]++;
    }

    for (int i = 0; i < 256; i++)
        brightness[i] = float(cbrightness[i]) / size;

    for (int i = 0; i < 256; i++)
        red[i] = float(cred[i]) / size;

    for (int i = 0; i < 256; i++)
        green[i] = float(cgreen[i]) / size;

    for (int i = 0; i < 256; i++)
        blue[i] = float(cblue[i]) / size;
}

}