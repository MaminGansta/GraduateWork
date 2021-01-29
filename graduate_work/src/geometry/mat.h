#pragma once

#include "vec.h"

#include <assert.h>
#include <algorithm>


namespace gm
{
	template <typename T>
	struct Mat
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

	public:
		Mat() = default;
		Mat(int width, int height, T* data = nullptr);
		Mat(std::initializer_list<Vec<T>> rows);
		
		pointer operator[] (int idx);
		const_pointer operator[] (int idx) const;
		Vec<T> GetCol(int idx) const;
		Vec<T> GetRow(int idx) const;
		void SetCol(int idx, const Vec<T>& vec);
		void SetRow(int idx, const Vec<T>& vec);


		pointer GetData() { return mData.data(); }
		const_pointer GetData() const { return mData.data(); }

		uint32_t GetColsNum() const { return std::min(mWidth,  (uint32_t)mData.size()); }
		uint32_t GetRowsNum() const { return std::min(mHeight, (uint32_t)mData.size()); }

		static Mat<T> GetIdentity(int wdith, int height);

	private:
		std::vector<T> mData;
		uint32_t mWidth;
		uint32_t mHeight;
	};

	typedef Mat<float> mat;

}