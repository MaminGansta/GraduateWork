#pragma once

#include "mat.h"

namespace gm
{
	template <typename T>
	Mat<T>::Mat(int width, int height, T* data)
		: mWidth(width),
		mHeight(height),
		mData(width* height)
	{}


	template <typename T>
	gm::Mat<T>::Mat(std::initializer_list<Vec<T>> rows)
	{
		mHeight = rows.size();
		mWidth = rows.begin()->GetSize();
		mData.resize(mWidth * mHeight);

		for (int i = 0; i < mHeight; i++)
		{
			for (int j = 0; j < mWidth; j++)
			{
				const Vec<T>& vec = *(rows.begin() + i);
				BUBBLE_ASSERT(vec.GetSize() == mWidth, "Invalid mat size");
				mData[i * mWidth + j] = vec[j];
			}
		}
	}


	template <typename T>
	T* Mat<T>::operator[](int idx)
	{
		BUBBLE_ASSERT(*(uint32_t*)&idx < mHeight, "Out of bound access");
		return mData.data() + mWidth * idx;
	}

	template <typename T>
	const T* Mat<T>::operator[](int idx) const
	{
		BUBBLE_ASSERT(*(uint32_t*)&idx < mHeight, "Out of bound access");
		return mData.data() + mWidth * idx;
	}

	template <typename T>
	Vec<T> Mat<T>::GetRow(int idx) const
	{
		BUBBLE_ASSERT(*(uint32_t*)&idx < GetRowsNum(), "Out of bound access");
		return Vec<T>(GetRowsNum(), mData.data() + GetRowsNum() * idx);
	}

	template <typename T>
	Vec<T> Mat<T>::GetCol(int idx) const
	{
		BUBBLE_ASSERT(*(uint32_t*)&idx < GetColsNum(), "Out of bound access");
		Vec<T> vec(GetColsNum());
		for (int i = 0; i < GetColsNum(); i++)
		{
			vec[i] = mData[mWidth * i + idx];
		}
		return std::move(vec);
	}

	template <typename T>
	void gm::Mat<T>::SetRow(int idx, const Vec<T>& row)
	{
		BUBBLE_ASSERT(*(uint32_t*)&idx < GetRowsNum(), "Out of bound access");
		BUBBLE_ASSERT(row.GetSize() == GetRowsNum(), "Out of bound access");

		memmove(mData.data() + GetColsNum() * idx, row.GetData(), sizeof(T) * row.GetSize());
	}


	template <typename T>
	void Mat<T>::SetCol(int idx, const Vec<T>& col)
	{
		BUBBLE_ASSERT(*(uint32_t*)&idx < GetColsNum(), "Out of bound access");
		BUBBLE_ASSERT(col.GetSize() == GetColsNum(), "Out of bound access");

		for (int i = 0; i < GetColsNum(); i++)
		{
			mData[mWidth * i + idx] = col[i];
		}
	}



	template <typename T>
	Mat<T> gm::Mat<T>::GetIdentity(int width, int height)
	{
		Mat<T> mat(width, height);
		for (int i = 0; i < std::min(width, height); i++)
		{
			mat[i][i] = T{ 1 };
		}
		return mat;
	}
}