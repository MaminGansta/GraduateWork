#pragma once

#include "pixel.h"
#include "cluster.h"
#include "data_structures/ms_params.h"

#include <cmath>
#include <vector>
#include <unordered_map>


namespace cpu
{
    struct MeanShiftPP
    {
        template <typename Point>
        static std::vector<Cluster<Point>> Run(const std::vector<Point>& points, MeanShitParams params)
        {
            std::vector<Point> active_buffer;
            std::vector<Point> sub_buffer;

            active_buffer = points;
            sub_buffer.resize(points.size());

            for (int i = 0; i < params.Iterations; i++)
            {
                // Size of grid cell
                float h = params.Radius / 3;
                std::unordered_map<std::array<int, Point::Dimensions()>, int> count;
                std::unordered_map<std::array<int, Point::Dimensions()>, Point> sum;
                for (const auto& point : points)
                {
                    auto grid_cell_id = GetCellByPoint(point, params);
                    sum[grid_cell_id] += point;
                    count[grid_cell_id] += 1;
                }

                // Calculate average
                for (auto& [key, value] : sum)
                {
                    value /= count[key];
                }
                auto& avg = sum;

                #pragma omp parallel for
                for (int i = 0; i < points.size(); i++)
                {
                    float total = 0;
                    Point mean_point;
                    auto grid_cell_id = GetCellByPoint(active_buffer[i], params);

                    for (int dim = 0; dim < Point::Dimensions(); dim++)
                    {
                        int coefs[] = { -1, 1, 1 };
                        for (int j = 0; j < 3; j++)
                        {
                            grid_cell_id[dim] += coefs[j];
                            if (avg.count(grid_cell_id))
                            {
                                total += 0.8f;
                                mean_point += avg[grid_cell_id] * 0.8f;
                            }
                        }

                        int coefs2[] = { -2, 0, 0, 0, 0, 4 };
                        for (int j = 0; j < 5; j++)
                        {
                            grid_cell_id[dim] += coefs2[j];
                            if (avg.count(grid_cell_id))
                            {
                                total += 0.5f;
                                mean_point += avg[grid_cell_id] * 0.5f;
                            }
                        }
                    }
                    mean_point /= total;
                    sub_buffer[i] = mean_point;
                }
                std::swap(active_buffer, sub_buffer);
            }
            return CreateClusters(points, active_buffer, params);
        }


        template <typename Point>
        static std::vector<Cluster<Point>> CreateClusters(const std::vector<Point>& points,
                                                          const std::vector<Point>& shifted_points,
                                                          MeanShitParams params)
        {
            std::vector<Cluster<Point>> clusters;
            clusters.reserve(points.size());

            for (int i = 0; i < points.size(); i++)
            {
                Cluster<Point>* cluster_min_dist = nullptr;
                float min_dist = std::numeric_limits<float>::max();

                for (auto& cluster : clusters)
                {
                    auto& shifted_point = shifted_points[i];
                    auto& centroid = cluster.GetCentroid();

                    float distance = shifted_point.Distance(centroid, params);
                    if (distance < params.Radius && distance < min_dist)
                    {
                        min_dist = distance;
                        cluster_min_dist = &cluster;
                    }
                }

                if (!cluster_min_dist)
                {
                    clusters.emplace_back(shifted_points[i]);
                    clusters.back().AddPoint(points[i]);
                }
                else
                    cluster_min_dist->AddPoint(points[i]);
            }
            return clusters;
        }

        template <typename Point>
        static auto GetCellByPoint(const Point& point, MeanShitParams params) -> std::array<int, Point::Dimensions()>
        {
            std::array<int, Point::Dimensions()> cell_id;

            cell_id[0] = params.DistanceCoef * point.vec[0] / (params.Radius / 9);
            cell_id[1] = params.DistanceCoef * point.vec[1] / (params.Radius / 9);
            cell_id[2] = params.ColorCoef * point.vec[2] / (params.Radius / 9);
            cell_id[3] = params.ColorCoef * point.vec[3] / (params.Radius / 9);
            cell_id[4] = params.ColorCoef * point.vec[4] / (params.Radius / 9);

            return cell_id;
        }
    };

}
