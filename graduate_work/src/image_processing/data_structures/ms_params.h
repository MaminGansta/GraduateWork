#pragma once 

namespace std
{
    template <size_t Dim>
    struct hash<std::array<int, Dim>>
    {
        std::size_t operator() (const std::array<int, Dim>& vec) const
        {
            int coefs[] = { 3, 13, 7, 11, 17 };

            int sum = 0;
            for (int i = 0; i < Dim; i++)
            {
                sum += coefs[i % 5] * vec[i];
            }
            return sum;
        }
    };

    template <size_t Dim>
    struct equal_to<std::array<int, Dim>>
    {
        bool operator() (const std::array<int, Dim>& vec1, const std::array<int, Dim>& vec2) const
        {
            for (int i = 0; i < Dim; i++)
                if (vec1[i] != vec2[i])
                    return false;
            return true;
        }
    };
}

struct MeanShitParams
{
    int Iterations = 5;
    int Radius = 0;
    int DistanceCoef = 0;
    int ColorCoef = 0;
    int BrightnessCoef = 0;
};