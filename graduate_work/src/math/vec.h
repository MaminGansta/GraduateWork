#pragma once

#include <cstdint>
#include <memory.h>
#include <cassert>
#include <vector>


namespace gm
{
	template <typename T>
	struct Vec
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

	public:
		Vec() = default;
		Vec(int size, const_pointer data = nullptr);
		
		reference operator[] (int idx);
		const_reference operator[](int idx) const;
		uint32_t GetSize() const { return mData.size(); }
		pointer GetData() { return mData.data(); }
		const_pointer GetData() const { return mData.data(); }

	private:
		std::vector<value_type> mData;
	};

	typedef Vec<float> vec;

}