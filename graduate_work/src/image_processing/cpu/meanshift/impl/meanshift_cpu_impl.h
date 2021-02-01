#pragma once

#include "cluster.h"

#include <vector>
#include <cmath>


namespace cpu
{
	struct MeanShift
	{
		template <typename Point>
		static std::vector<Cluster<Point>> Run(const std::vector<Point>& points,
											   float radius,
											   int max_interations = 100);

		template <typename Point>
		static std::vector<Cluster<Point>> CreateClusters(const std::vector<Point>& points,
														  const std::vector<Point>& shifted_points,
														  float radius);
	};

}
