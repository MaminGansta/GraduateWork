#pragma once

#include "vec_view.h"

namespace gm
{
	template <typename T>
	VecView<T>::VecView(int size, T* data)
		: mHead(data),
		mSize(size)
	{}

	template <typename T>
	gm::VecView<T>::VecView(const Vec<T>& vec)
		: mHead(vec.GetData()),
		mSize(vec.GetSize())
	{}

	template <typename T>
	T& VecView<T>::operator[](int idx)
	{
		BUBBLE_ASSERT(*(uint32_t*)&idx < GetSize(), "Out of bound access");
		return mHead[idx];
	}

	template <typename T>
	const T& VecView<T>::operator[](int idx) const
	{
		BUBBLE_ASSERT(*(uint32_t*)&idx < GetSize(), "Out of bound access");
		return mHead[idx];
	}

}