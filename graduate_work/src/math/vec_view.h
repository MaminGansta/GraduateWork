#pragma once

#include <cstdint>
#include <cassert>

#include "vec.h"


namespace gm
{
	template <typename T>
	struct VecView
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

	public:
		VecView() = default;
		VecView(int size, T* data);
		VecView(const Vec<T>& vec);

		T& operator[] (int idx);
		const T& operator[](int idx) const;
		uint32_t GetSize() const { return mSize; }

	private:
		T* mHead;
		uint32_t mSize;
	};

}

