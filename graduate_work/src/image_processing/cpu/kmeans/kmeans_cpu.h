#pragma once

#include "kmeans_cluster.h"

#include <functional>
#include <random>


namespace cpu
{
	template <typename Point>
	using DistaceRateFunc = std::function<float(const Point&, const Point&)>;


	// ================ Kmeans realization ================

	template <typename Point>
	static void Init(const std::vector<Point>& points, std::vector<Cluster<Point>>& clusters, int clusters_num)
	{
		clusters.resize(clusters_num);

		for (auto& cluster : clusters)
		{
			cluster.mPoints.reserve(points.size() / clusters_num);
		}

		// Init clusters' positions
		std::vector<int> cache;
		cache.reserve(clusters_num);
		std::srand(std::time(nullptr));

		for (auto& cluster : clusters)
		{
			int pos = std::rand() % points.size();
			while (std::find(cache.begin(), cache.end(), pos))
			{
				pos++;
			}
			cache.push_back(pos);
			cluster.mCenter = points[pos];
		}
	}

	template <typename Point>
	static void SplitPointsbyClusters(const std::vector<Point>& points,
									  std::vector<Cluster<Point>>& clusters,
									  const DistaceRateFunc<Point>& distance_rate)
	{
		for (auto& point : points)
		{
			Cluster<Point>* nearest_cluster;
			float min_distance = (std::numeric_limits<float>::max)();

			for (auto& cluster : clusters)
			{
				float distance = distance_rate(point, cluster.mCenter);
				if (distance < min_distance)
				{
					nearest_cluster = &cluster;
					min_distance = distance;
				}
			}

			nearest_cluster->mPoints.push_back(point);
		}
	}

	template <typename Point>
	static bool ComputeMeansAndMove(std::vector<Cluster<Point>>& clusters, 
									const DistaceRateFunc<Point>& distance_rate)
	{
		bool end = false;

		for (auto& cluster : clusters)
		{
			Point mean{ 0 };
			for (auto& point : cluster)
			{
				mean += point;
			}
			mean /= cluster.Size();
			float diff = distance_rate(cluster.mCenter, mean);
			end |= diff < 0.01f;
			cluster.mCenter = mean;
		}

		return end;
	}

	// =================== Kmeans ===================
	/*
	* Point must have
	* operator +=, /=
	*/
	template <typename Point>
	std::vector<Cluster<Point>>
		KMeans(std::vector<Point>& points,
			   const std::function<float(const Point&, const Point&)>& distance_rate,
			   int clusters_num,
			   int max_iterations = 100)
	{
		std::vector<Cluster> clusters;
		Init(points, clusters, clusters_num);

		for (int i = 0; i < max_iterations; i++)
		{
			for (auto& cluster : clusters)
			{
				cluster.mPoints.clear();
			}

			SplitPointsbyClusters<Point>(points, clusters);
			bool end = ComputeMeansAndMove<Point>(clusters);

			if (end)
			{
				break;
			}
		}

		return clusters;
	}

}