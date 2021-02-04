#pragma once

#include "image_processing.h"
#include "kmeans_cluster.h"
#include "pixel.h"

#include <functional>
#include <algorithm>


using PixelDistaceRate = std::function<float(const Pixel&, const Pixel&)>;

namespace cpu
{
	static void Init(const cpu::Image& image,
					 std::vector<KMeansCluster<Pixel>>& clusters,
					 int clusters_num);

	static void SplitPointsbyClusters(const cpu::Image& image,
									  std::vector<Cluster<Pixel>>& clusters,
									  const PixelDistaceRate& distance_rate);

	static bool ComputeMeansAndMove(const Image& image,
									std::vector<KMeansCluster<Pixel>>& clusters,
									const PixelDistaceRate& distance_rate);

	static int GetImageSize(const Image& image);
	static Pixel GetImagePixel(const Image& image, int pos);


	// ================ Image kmeans ================ 

	inline std::vector<KMeansCluster<Pixel>>
		ImageKMeans(const cpu::Image& image,
			const PixelDistaceRate& distance_rate,
			int clusters_num,
			int max_iterations)
	{
		std::vector<KMeansCluster<Pixel>> clusters;
		Init(image, clusters, clusters_num);

		for (int i = 0; i < max_iterations; i++)
		{
			for (auto& cluster : clusters)
			{
				cluster.Clear();
			}

			SplitPointsbyClusters(image, clusters, distance_rate);
			bool end = ComputeMeansAndMove(image, clusters, distance_rate);

			if (end)
			{
				break;
			}
		}

		return clusters;
	}


	static void Init(const cpu::Image& image, std::vector<KMeansCluster<Pixel>>& clusters, int clusters_num)
	{
		clusters.resize(clusters_num);

		for (auto& cluster : clusters)
		{
			cluster.mPoints.reserve(GetImageSize(image) / clusters_num);
		}

		// Init clusters' positions
		std::vector<int> cache;
		cache.reserve(clusters_num);
		std::srand(std::time(nullptr));

		for (auto& cluster : clusters)
		{
			int pos = std::rand() % GetImageSize(image);

			while (std::find(cache.begin(), cache.end(), pos) != cache.end())
			{
				pos = (pos + 1) % GetImageSize(image);
			}
			cache.push_back(pos);
			cluster.mCenter = GetImagePixel(image, pos);
		}
	}

	static void SplitPointsbyClusters(const cpu::Image& image,
									  std::vector<KMeansCluster<Pixel>>& clusters,
									  const PixelDistaceRate& distance_rate)
	{
		for (int i = 0; i < GetImageSize(image); i++)
		{
			Pixel pixel = GetImagePixel(image, i);
			KMeansCluster<Pixel>* nearest_cluster;
			float min_distance = std::numeric_limits<float>::max();

			for (auto& cluster : clusters)
			{
				float distance = distance_rate(pixel, cluster.mCenter);
				if (distance < min_distance)
				{
					nearest_cluster = &cluster;
					min_distance = distance;
				}
			}
			nearest_cluster->AddPoint(pixel);
		}
	}

	static bool ComputeMeansAndMove(const Image& image,
									std::vector<KMeansCluster<Pixel>>& clusters,
								    const PixelDistaceRate& distance_rate)
	{
		bool end = false;

		for (auto& cluster : clusters)
		{
			Pixel mean;
			mean.channels = image.GetChannels();

			for (auto& pixel : cluster)
			{
				mean += pixel;
			}
			mean /= cluster.Size();
			float diff = distance_rate(cluster.mCenter, mean);
			end |= diff < 1.0f; // step too small
			cluster.mCenter = mean;
		}
		return end;
	}


	static int GetImageSize(const Image& image)
	{
		return image.GetHeight() * image.GetWidth();
	}

	static Pixel GetImagePixel(const Image& image, int pos)
	{
		Pixel pixel;
		pixel.channels = image.GetChannels();
		pixel.y = pos / image.GetWidth();
		pixel.x = pos % image.GetWidth();

		uint8_t* color = image.GetData() + pos * pixel.channels;
		for (int i = 0; i < 4; i++)
		{
			pixel.color[i] = color[i];
		}
		return pixel;
	}

}