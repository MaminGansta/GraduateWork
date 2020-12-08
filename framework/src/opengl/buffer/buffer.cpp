
#include "buffer.h"


namespace Bubble
{
	// ==================== Vertex buffer ======================

	VertexBuffer::VertexBuffer(uint32_t size)
		: mSize(size)
	{
		glcall(glGenBuffers(1, &mRendererID));
		glcall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
		glcall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	}

	VertexBuffer::VertexBuffer(void* vertices, uint32_t size)
		: mSize(size)
	{
		glcall(glGenBuffers(1, &mRendererID));
		glcall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
		glcall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	{
		mRendererID = other.mRendererID;
		mLayout = std::move(other.mLayout);
		mSize = other.mSize;
		other.mRendererID = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
	{
		if (this != &other)
		{
			mRendererID = other.mRendererID;
			mLayout = std::move(other.mLayout);
			mSize = other.mSize;
			other.mRendererID = 0;
			other.mSize = 0;
		}
		return *this;
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &mRendererID);
	}

	void VertexBuffer::Bind() const
	{
		glcall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
	}

	void VertexBuffer::Unbind() const
	{
		glcall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		glcall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
		glcall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}

	const Bubble::BufferLayout& VertexBuffer::GetLayout() const
	{
		return mLayout;
	}

	void VertexBuffer::SetLayout(const BufferLayout& layout)
	{
		mLayout = layout;
	}


	// ===================== Index buffer ======================== 

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
		: mCount(count)
	{
		glcall(glGenBuffers(1, &mRendererID));
		glcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID));
		glcall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
	{
		mRendererID = other.mRendererID;
		mCount = other.mCount;
		other.mRendererID = 0;
		other.mCount = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
	{
		if (this != &other)
		{
			mRendererID = other.mRendererID;
			mCount = other.mCount;
			other.mRendererID = 0;
			other.mCount = 0;
			return *this;
		}
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &mRendererID);
	}

	void IndexBuffer::Bind() const
	{
		glcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID));
	}

	void IndexBuffer::Unbind() const
	{
		glcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	uint32_t IndexBuffer::GetCount() const
	{
		return mCount;
	}

}
