#pragma once

#include <random>
#include <array>
#include <cmath>
#include <cassert>
#include <limits>
#include <type_traits>


class RNG
{
	std::array<uint64_t, 16> mState;
	uint32_t mPosition;
	std::random_device mRandomDevice;

public:
	RNG(const std::array<uint64_t, 16>& seed)
        : mState(seed),
          mPosition(0)
    {}

    RNG()
        : mPosition(0)
    {
        for(uint64_t &s : mState)
        {
            if(std::numeric_limits<unsigned int>::digits >= 64)
            {
                s = mRandomDevice();
            }
            else if(std::numeric_limits<unsigned int>::digits >= 32) {
                s = mRandomDevice() & 0xffffffff;
                s |= static_cast<uint64_t>(mRandomDevice()) << 32;
            }
            else
            {
                s = mRandomDevice() & 0xffff;
                s |= (static_cast<uint64_t>(mRandomDevice()) << 16) & 0xffffffff;
                s |= (static_cast<uint64_t>(mRandomDevice()) << 32) & 0xffffffffffff;
                s |= (static_cast<uint64_t>(mRandomDevice()) << 48);
            }
        }
    }

    void SetSeed(const std::array<uint64_t, 16> &seed)
    {
        mState = seed;
    }

    const std::array<uint64_t, 16>& GetState() const
    {
        return mState;
    }

    /*
        Right boundary is not included
    */
    template <typename T>
	T Get(const T& min_value = std::numeric_limits<T>::min(),
          const T& max_value = std::numeric_limits<T>::max())
	{
        // Integral type
        if constexpr (std::is_integral_v<T>)
        {
            T range = max_value - min_value;
            if constexpr (std::is_signed_v<T>)
            {
                range = std::abs(range);
            }
		    T rand = GetRandomUint64() & std::numeric_limits<T>::max();
		    return rand % range + min_value;
		}
        // Floating point type
        else if constexpr (std::is_floating_point_v<T>)
        {
            T range = std::abs(max_value - min_value);
            return GetRandomFloatType<T>() * range + min_value;
        }
        else {
            static_assert(false);
        }
	}


private:

    uint64_t GetRandomUint64()
    {
        return xorshift1024();
    }

    template <class T>
    T GetRandomFloatType()
    {
        constexpr T epsilon = std::numeric_limits<T>::epsilon();
        constexpr int digitsToKeep = std::numeric_limits<T>::digits-1;
        constexpr int digitsToLose = std::numeric_limits<uint64_t>::digits - digitsToKeep;
        T rand = (xorshift1024() >> digitsToLose) * epsilon;
        return rand;
    }

    uint64_t xorshift1024()
    {
        uint64_t state0 = mState[mPosition];
        mPosition = (mPosition + 1) % 16;
        uint64_t state1 = mState[mPosition];
        state1 ^= state1 << 31;
        state1 ^= state1 >> 11;
        state0 ^= state0 >> 30;
        mState[mPosition] = state0 ^ state1;
        return mState[mPosition] * 1181783497276652981LL;
    }

    int countLeadingZeros64(const uint64_t &toCount)
    {
    #ifdef __GNUC__
        return __builtin_clzll(toCount) - (std::numeric_limits<uint32_t long>::digits - 64);
    #elif defined(_MSC_VER)
        return __lzcnt(toCount);
    #else
        static const int clz64Index[64] = {
            63, 16, 62,  7, 15, 36, 61,  3,
             6, 14, 22, 26, 35, 47, 60,  2,
             9,  5, 28, 11, 13, 21, 42, 19,
            25, 31, 34, 40, 46, 52, 59,  1,
            17,  8, 37,  4, 23, 27, 48, 10,
            29, 12, 43, 20, 32, 41, 53, 18,
            38, 24, 49, 30, 44, 33, 54, 39,
            50, 45, 55, 51, 56, 57, 58,  0
        };
        uint64_t bitset = toCount;
        bitset |= bitset >> 1;
        bitset |= bitset >> 2;
        bitset |= bitset >> 4;
        bitset |= bitset >> 8;
        bitset |= bitset >> 16;
        bitset |= bitset >> 32;
        return clz64Index[(bitset * 0x03f79d71b4cb0a89ULL) >> 58];
    #endif
    }

};
