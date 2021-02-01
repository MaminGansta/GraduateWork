#pragma once

#include "bubble.h"
#include "image_processing.h"


std::vector<uint8_t> RandomColor()
{
    RNG rng;
    std::vector<uint8_t> res(4);
    for (int i = 0; i < 3; i++)
    {
        res[i] = rng.Get<uint8_t>();
    }
    res[3] = 255;
    return res;
}

inline std::vector<std::vector<uint8_t>> GetRandomColors(int size)
{
    std::vector<std::vector<uint8_t>> colors;
    for (int i = 0; i < size; i++)
    {
        colors.push_back(RandomColor());
    }
    return colors;
}

inline gpu::Image GetImageFromClusters(const std::vector<Cluster<Pixel>>& clusters, Texture2DSpecification spec)
{
    cpu::Image image(spec);
    auto colors = GetRandomColors(clusters.size());

    for (int i = 0; i < clusters.size(); i++)
    {
        for (const Pixel& pixel : clusters[i])
        {
            auto color = colors[i];
            image.SetColor(color.data(), pixel.x, pixel.y);
        }
    }
    return image.LoadOnGPU();
}

inline std::vector<Ref<gpu::Image>> GetRefImagesFromPixelData(const std::vector<std::vector<Pixel>>& snapshots, Texture2DSpecification spec)
{
    std::vector<Ref<gpu::Image>> images;
    for (const auto& snapshot : snapshots)
    {
        cpu::Image snapshot_image(spec);
        for (auto& pixel : snapshot)
        {
            uint8_t color[] = { pixel.color[0], pixel.color[1], pixel.color[2] };
            snapshot_image.SetColor(color, pixel.x, pixel.y);
        }
        images.push_back(CreateRef<gpu::Image>(snapshot_image.LoadOnGPU()));
    }
    return images;
}




