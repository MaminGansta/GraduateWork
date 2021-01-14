
#include "meanshift_cpu_impl.h"

#include "bubble.h"
#include <omp.h>

template <typename Point>
std::vector<Cluster<Point>> MeanShift::Run(const std::vector<Point>& points,
										   float radius,
										   int max_interations)
{
	std::vector<Point> buffers[2];
	uint32_t active_buffer_id = 0;
	uint32_t sub_buffer_id = 1;
	bool shifting = true;

	buffers[active_buffer_id] = points;
	buffers[sub_buffer_id].resize(points.size());

	for (int i = 0; i < max_interations && shifting; i++)
	{
		LOG_INFO("iteration {}", i);
		auto& active_buffer = buffers[active_buffer_id];
		auto& sub_buffer = buffers[sub_buffer_id];
		shifting = false;

		#pragma omp parallel for
		for (int i = 0; i < points.size(); i++)
		{
			Point& active_point = active_buffer[i];
			Point mean_point;
			float total = 0.0f;

			int pad = points.size();
			int from = std::max(i - pad, 0);
			int to = std::min(i + pad, (int)points.size());

			//for (const auto& point : active_buffer)
			for (int j = from; j < to; j++)
			{
				const auto& point = active_buffer[j];
				float distance = active_point.Distance(point);

				if (distance < radius * 3)
				{
					float influance = std::exp(-(distance * distance) / (2.0f * radius * radius));
					//float influance = 1.0f;
					mean_point += point * influance; 
					total += influance;
				}
			}
			mean_point /= total;
			sub_buffer[i] = mean_point;

			//float shift = active_point.Distance(mean_point);
			//shifting |= shift > 1.0f;
			//shifting = true;

			//static int a = 0;
			//if (i / 100 > a)
			//{
			//	a = i / 100;
			//	LOG_INFO("{}", a);
			//}
		}

		std::swap(active_buffer_id, sub_buffer_id);
	}

	return CreateClusters(points, buffers[active_buffer_id], radius);
}



template <typename Point>
std::vector<Cluster<Point>> MeanShift::CreateClusters(const std::vector<Point>& points,
													  const std::vector<Point>& shifted_points,
													  float radius)
{
	std::vector<Cluster<Point>> clusters;
	clusters.reserve(points.size());

	for (int i = 0; i < points.size(); i++)
	{
		bool separate_point = true;

		for (auto& cluster : clusters)
		{
			auto& shifted_point = shifted_points[i];
			auto& centroid = cluster.GetCentroid();
			
			if (shifted_point.Distance(centroid) < radius)
			{
				cluster.AddPoint(points[i]);
				separate_point = false;
			}
		}

		if (separate_point)
		{
			clusters.emplace_back(shifted_points[i]);
			clusters.back().AddPoint(points[i]);
		}
	}

	return std::move(clusters);
}
