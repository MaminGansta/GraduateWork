#pragma once

#include <vector>

template <typename Point>
struct Cluster
{
    Point mCentroid;
    std::vector<Point> mPoints;

    void AddPoint(const Point& point) { mPoints.push_back(point); }
    void Clear() { mPoints.clear(); }

    Point& GetCentroid() { return mCentroid; }
    auto& GetPoints() { return mPoints; }
    int Size() const { return mPoints.size(); }

    auto begin() { return mPoints.begin(); }
    auto end() { return mPoints.end(); }

    auto begin() const { return mPoints.cbegin(); }
    auto end() const { return mPoints.cend(); }
};