#pragma once

#include "vec.h"

namespace gm
{
	template <typename T>
	Vec<T>::Vec(int size, const_pointer data)
	{
		mData.resize(size);
		if (data)
		{
			memmove(mData.data(), data, sizeof(T) * size);
		}
	}

	template <typename T>
	gm::Vec<T>::Vec(std::initializer_list<T> values)
		: mData(values)
	{}

	template <typename T>
	T& Vec<T>::operator[](int idx)
	{
		BUBBLE_ASSERT(*(uint32_t*)&idx < GetSize(), "Out of bound access");
		return mData[idx];
	}

	template <typename T>
	const T& Vec<T>::operator[](int idx) const
	{
		BUBBLE_ASSERT(*(uint32_t*)&idx < GetSize(), "Out of bound access");
		return mData[idx];
	}
}