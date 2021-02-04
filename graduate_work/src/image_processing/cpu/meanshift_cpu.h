#pragma once

#include "pixel.h"
#include "cluster.h"

#include <cmath>
#include <vector>

namespace cpu
{
    struct MeanShift
    {
        template <typename Point>
        static std::vector<Cluster<Point>> Run(const std::vector<Point>& points,
                                               float radius,
                                               int max_interations)
        {
            std::vector<Point> buffers[2];
            uint32_t active_buffer_id = 0;
            uint32_t sub_buffer_id = 1;

            buffers[active_buffer_id] = points;
            buffers[sub_buffer_id].resize(points.size());

            for (int i = 0; i < max_interations; i++)
            {
                LOG_INFO("iteration {}", i);
                auto& active_buffer = buffers[active_buffer_id];
                auto& sub_buffer = buffers[sub_buffer_id];

                #pragma omp parallel for
                for (int i = 0; i < points.size(); i++)
                {
                    Point& active_point = active_buffer[i];
                    Point mean_point;
                    float total = 0.0f;

                    for (const auto& point : active_buffer)
                    {
                        float distance = active_point.Distance(point);

                        if (distance < radius * 3)
                        {
                            float influance = std::exp(-(distance * distance) / (2.0f * radius * radius));
                            mean_point += point * influance;
                            total += influance;
                        }
                    }
                    mean_point /= total;
                    sub_buffer[i] = mean_point;
                }

                std::swap(active_buffer_id, sub_buffer_id);
            }

            return CreateClusters(points, buffers[active_buffer_id], radius);
        }


        template <typename Point>
        static std::vector<Cluster<Point>> CreateClusters(const std::vector<Point>& points,
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
    };

}
