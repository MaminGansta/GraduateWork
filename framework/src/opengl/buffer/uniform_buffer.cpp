
#include "uniform_buffer.h"


namespace Bubble
{
	// ============================= UniformBuffer ============================= 

	UniformBuffer::UniformBuffer(int index, const BufferLayout& layout, uint32_t size, uint32_t additional_size)
		: mLayout(std::move(layout)),
		  mIndex(index),
		  mSize(size)
	{
		CalculateOffsetsAndStride();
		mBufferSize = mLayout.mStride * size;

		glGenBuffers(1, &mRendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, mRendererID);
		glBufferData(GL_UNIFORM_BUFFER, mBufferSize, NULL, GL_DYNAMIC_DRAW);

		glBindBufferRange(GL_UNIFORM_BUFFER, index, mRendererID, 0, mBufferSize);
	}

	UniformBuffer::UniformBuffer(int index, uint32_t buffer_size)
		:mIndex(index),
		 mBufferSize(buffer_size)
	{
		glGenBuffers(1, &mRendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, mRendererID);
		glBufferData(GL_UNIFORM_BUFFER, mBufferSize, NULL, GL_DYNAMIC_DRAW);

		glBindBufferRange(GL_UNIFORM_BUFFER, index, mRendererID, 0, mBufferSize);
	}

	UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept
		: mRendererID(other.mRendererID),
		  mBufferSize(other.mBufferSize),
		  mLayout(std::move(other.mLayout)),
     	  mIndex(other.mIndex),
		  mSize(other.mSize)
	{
		other.mRendererID = 0;
		other.mSize = 0;
		other.mBufferSize = 0;
	}

	UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept
	{
		if (this != &other)
		{
			mRendererID = other.mRendererID;
			mBufferSize = other.mBufferSize;
			mLayout = std::move(other.mLayout);
			mIndex = other.mIndex;
			mSize = other.mSize; 

			other.mRendererID = 0;
			other.mSize = 0;
			other.mBufferSize = 0;
		}
		return *this;
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &mRendererID);
	}

	void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, mRendererID);
		glcall(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
	}

	UniformArrayElemnt UniformBuffer::operator[](int index)
	{
		BUBBLE_CORE_ASSERT(*(uint32_t*)&index < mSize, "Buffer acess valiation");
		return UniformArrayElemnt(*this, index);
	}

	void UniformBuffer::CalculateOffsetsAndStride()
	{
		uint32_t offset = 0;
		uint32_t pad = 0;  // padding in std140

		for (BufferElement& elemnt : mLayout)
		{
			uint32_t std140_pad = Std140DataTypePadding(elemnt.Type);
			elemnt.Size = Std140DataTypeSize(elemnt.Type);
			
			pad = offset % std140_pad;
			pad = pad > 0 ? std140_pad - pad : 0;

			elemnt.Offset = offset + pad;
			offset += elemnt.Size + pad;
		}
		// Align by vec4 size
		uint32_t vec4_size = Std140DataTypeSize(GLSLDataType::Float4);
		pad = offset % vec4_size;
		offset += pad > 0 ? vec4_size - pad : 0;

		mLayout.mStride = offset;
	}


	// ============================= UniformArrayElemnt ============================= 

	UniformArrayElemnt::UniformArrayElemnt(const UniformBuffer& uniform_buffer, int index)
		: mLayout(&uniform_buffer.mLayout),
		  mRendererID(uniform_buffer.mRendererID),
		  mArrayIndex(index)
	{}

	void UniformArrayElemnt::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		int array_index_offset = mLayout->mStride * mArrayIndex;
		size = size ? size : mLayout->mStride;
		glBindBuffer(GL_UNIFORM_BUFFER, mRendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, array_index_offset + offset, size, data);
	}


	void UniformArrayElemnt::SetInt(const std::string& name, int data)
	{
		BufferElement* elem = FindBufferElement(name, GLSLDataType::Int);
		SetRawData(elem, &data);
	}


	void UniformArrayElemnt::SetFloat(const std::string& name, float data)
	{
		BufferElement* elem = FindBufferElement(name, GLSLDataType::Float);
		SetRawData(elem, &data);
	}


	void UniformArrayElemnt::SetFloat2(const std::string& name, const glm::vec2& data)
	{
		BufferElement* elem = FindBufferElement(name, GLSLDataType::Float2);
		SetRawData(elem, glm::value_ptr(data));
	}


	void UniformArrayElemnt::SetFloat3(const std::string& name, const glm::vec3& data)
	{
		BufferElement* elem = FindBufferElement(name, GLSLDataType::Float3);
		SetRawData(elem, glm::value_ptr(data));
	}


	void UniformArrayElemnt::SetFloat4(const std::string& name, const glm::vec4& data)
	{
		BufferElement* elem = FindBufferElement(name, GLSLDataType::Float4);
		SetRawData(elem, glm::value_ptr(data));
	}


	void UniformArrayElemnt::SetMat4(const std::string& name, const glm::mat4& data)
	{
		BufferElement* elem = FindBufferElement(name, GLSLDataType::Mat4);
		SetRawData(elem, glm::value_ptr(data));
	}



	BufferElement* UniformArrayElemnt::FindBufferElement(const std::string& name, GLSLDataType type)
	{
		auto elem = std::find_if(mLayout->begin(), mLayout->end(),
			[&name, &type](const BufferElement& elem)
			{
				return elem.Name == name && elem.Type == type;
			}
		);
		BUBBLE_CORE_ASSERT(elem != mLayout->end(), "Uniform buffer element not founded: " + name);
		return elem._Ptr;
	}

	void UniformArrayElemnt::SetRawData(BufferElement* elem, const void* data)
	{
		int array_index_offset = mLayout->mStride * mArrayIndex + elem->Offset;
		glBindBuffer(GL_UNIFORM_BUFFER, mRendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, array_index_offset, elem->Size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

}