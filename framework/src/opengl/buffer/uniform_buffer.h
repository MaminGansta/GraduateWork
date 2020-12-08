#pragma once

#include "buffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Bubble
{
	struct UniformArrayElemnt;

	inline uint32_t Std140DataTypeSize(GLSLDataType type)
	{
		switch (type)
		{
			case GLSLDataType::Float:  return 4;
			case GLSLDataType::Float2: return 4 * 2;
			case GLSLDataType::Float3: return 4 * 4;
			case GLSLDataType::Float4: return 4 * 4;
			case GLSLDataType::Mat3:   return 4 * 4 * 4;
			case GLSLDataType::Mat4:   return 4 * 4 * 4;
			case GLSLDataType::Int:    return 4;
			case GLSLDataType::Int2:   return 4 * 2;
			case GLSLDataType::Int3:   return 4 * 4;
			case GLSLDataType::Int4:   return 4 * 4;
			case GLSLDataType::Bool:   return 4;
		}
		BUBBLE_CORE_ASSERT(false, "Unknown GLSLDataType!");
		return 0;
	}


	inline uint32_t Std140DataTypePadding(GLSLDataType type)
	{
		switch (type)
		{
			case GLSLDataType::Float:  return 4;
			case GLSLDataType::Float2: return 8;
			case GLSLDataType::Float3: return 16;
			case GLSLDataType::Float4: return 16;
			case GLSLDataType::Mat3:   return 16;
			case GLSLDataType::Mat4:   return 16;
			case GLSLDataType::Int:    return 4;
			case GLSLDataType::Int2:   return 8;
			case GLSLDataType::Int3:   return 16;
			case GLSLDataType::Int4:   return 16;
			case GLSLDataType::Bool:   return 4;
		}
		BUBBLE_CORE_ASSERT(false, "Unknown GLSLDataType!");
		return 0;
	}


	struct UniformBuffer
	{
		uint32_t mRendererID = 0;
		uint32_t mBufferSize = 0;
		BufferLayout mLayout;
		uint32_t mSize; // elements in array
		uint32_t mIndex;

		UniformBuffer() = default;
		// additional size necessary if buffer contain more then one array (for example nLights)
		UniformBuffer(int index, const BufferLayout& layout = {}, uint32_t size = 1, uint32_t additional_size = 0);
		UniformBuffer(int index, uint32_t size);

		UniformBuffer(const UniformBuffer&) = delete;
		UniformBuffer& operator=(const UniformBuffer&) = delete;

		UniformBuffer(UniformBuffer&&) noexcept;
		UniformBuffer& operator=(UniformBuffer&&) noexcept;

		~UniformBuffer();
		
		// Raw (Don't forget to observe std140 paddings)
		void SetData(const void* data, uint32_t size, uint32_t offset = 0);

		// Save (Use it event only one element in buffer)
		UniformArrayElemnt operator[] (int index);

		const BufferLayout& GetLayout() const { return mLayout; };
		// Return size in bytes
		uint32_t GetBufferSize() { return mBufferSize; }
		// Return size of elements
		uint32_t GetSize() { return mSize; };

	private:
		// Recalculate size and offset of elements for std140
		void CalculateOffsetsAndStride();
	};


	/*
		Doesn't own any resources
		Point to current element in array
	*/
	struct UniformArrayElemnt
	{
		uint32_t mRendererID = 0;
		uint32_t mArrayIndex = 0;
		const BufferLayout* mLayout;

		UniformArrayElemnt(const UniformBuffer& uniform_buffer, int index);

		// Raw
		void SetData(const void* data, uint32_t size = 0, uint32_t offset = 0);

		// Save
		void SetInt   (const std::string& name, int   data);
		void SetFloat (const std::string& name, float data);
		void SetFloat2(const std::string& name, const glm::vec2& data);
		void SetFloat3(const std::string& name, const glm::vec3& data);
		void SetFloat4(const std::string& name, const glm::vec4& data);
		void SetMat4  (const std::string& name, const glm::mat4& data);

	private:
		BufferElement* FindBufferElement(const std::string& name, GLSLDataType type);
		void SetRawData(BufferElement* elem, const void* data);
	};

}