#pragma once

#include <vector>

template <typename Point>
struct Cluster
{
	Point mCenter;
	std::vector<Point> mPoints;

	int Size() const { return mPoints.size(); }
	auto begin() { return mPoints.begin(); }
	auto end() { return mPoints.end(); }
};