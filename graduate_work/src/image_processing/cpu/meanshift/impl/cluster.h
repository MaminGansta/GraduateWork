#pragma once

template <typename Point>
struct Cluster
{
	Point mCentroid;
	std::vector<Point> mPoints;

	void AddPoint(const Point& point) { mPoints.push_back(point); }
	Point& GetCentroid() { return mCentroid; }
	auto& GetPoints() { return mPoints; }
	void Clear() { mPoints.clear(); }
	int Size() const { return mPoints.size(); }
	auto begin() { return mPoints.begin(); }
	auto end() { return mPoints.end(); }
};