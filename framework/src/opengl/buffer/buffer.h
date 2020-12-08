#pragma once

#include "renderer_base.h"

#include <vector>
#include <string>
#include <cassert>


namespace Bubble
{
	enum class GLSLDataType
	{
		None, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	inline uint32_t GLSLDataTypeSize(GLSLDataType type)
	{
		switch (type)
		{
			case GLSLDataType::Float:  return 4;
			case GLSLDataType::Float2: return 4 * 2;
			case GLSLDataType::Float3: return 4 * 3;
			case GLSLDataType::Float4: return 4 * 4;
			case GLSLDataType::Mat3:   return 4 * 3 * 3;
			case GLSLDataType::Mat4:   return 4 * 4 * 4;
			case GLSLDataType::Int:    return 4;
			case GLSLDataType::Int2:   return 4 * 2;
			case GLSLDataType::Int3:   return 4 * 3;
			case GLSLDataType::Int4:   return 4 * 4;
			case GLSLDataType::Bool:   return 1;
		}
		BUBBLE_CORE_ASSERT(false, "Unknown GLSLDataType!");
		return 0;
	}


	struct BufferElement
	{
		std::string Name;
		GLSLDataType Type;
		uint32_t Size;
		uint32_t Count;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(GLSLDataType type, const std::string & name, size_t count = 1, bool normalized = false)
			: Name(name),
			  Type(type),
			  Size(GLSLDataTypeSize(type)),
			  Offset(0),
			  Count(count),
			  Normalized(normalized)
		{}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case GLSLDataType::Float:  return 1;
				case GLSLDataType::Float2: return 2;
				case GLSLDataType::Float3: return 3;
				case GLSLDataType::Float4: return 4;
				case GLSLDataType::Mat3:   return 3;
				case GLSLDataType::Mat4:   return 4;
				case GLSLDataType::Int:    return 1;
				case GLSLDataType::Int2:   return 2;
				case GLSLDataType::Int3:   return 3;
				case GLSLDataType::Int4:   return 4;
				case GLSLDataType::Bool:   return 1;
			}

			BUBBLE_CORE_ASSERT(false, "Unknown GLSLDataType!");
			return 0;
		}
	};


	struct BufferLayout
	{
		std::vector<BufferElement> mElements;
		uint32_t mStride = 0;

		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: mElements(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return mStride; }
		const std::vector<BufferElement>& GetElements() const { return mElements; }

		std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
		std::vector<BufferElement>::iterator end() { return mElements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			for (auto& element : mElements)
			{
				element.Offset = offset;
				offset += element.Size * element.Count;
				mStride += element.Size;
			}
			// If count more then one, it means that
			// attributes goes one after another (1111 2222 3333)
			// So stride will be the size of each single attribute
			// that will be set in vertex array class
			mStride = mElements[0].Count == 1 ? mStride : 0;
		}
	};


	struct VertexBuffer
	{
		uint32_t mRendererID = 0;
		uint32_t mSize = 0;
		BufferLayout mLayout;

		VertexBuffer() = default;
		VertexBuffer(uint32_t size);
		VertexBuffer(void* vertices, uint32_t size);

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&&) noexcept;
		VertexBuffer& operator=(VertexBuffer&&) noexcept;

		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, uint32_t size);
		 
		const BufferLayout& GetLayout() const;
		void SetLayout(const BufferLayout& layout);

		uint32_t GetSize() { return mSize; }
	};

	
	struct IndexBuffer
	{
		uint32_t mRendererID = 0;
		uint32_t mCount = 0;

		IndexBuffer() = default;
		IndexBuffer(uint32_t* indices, uint32_t count);

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		IndexBuffer(IndexBuffer &&) noexcept;
		IndexBuffer& operator=(IndexBuffer&&) noexcept;

		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const;
	};
}