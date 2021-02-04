#pragma once

#include <vector>

template <typename Point>
struct KMeansCluster
{
	Point mCenter;
	std::vector<Point> mPoints;

	void AddPoint(const Point& point) { mPoints.push_back(point); }
	void Clear() { mPoints.clear(); }
	int  Size() const { return mPoints.size(); }
	auto begin() { return mPoints.begin(); }
	auto end() { return mPoints.end(); }
};