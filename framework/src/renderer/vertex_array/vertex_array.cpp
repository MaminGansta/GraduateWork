
#include "vertex_array.h"

namespace Bubble
{
	static GLenum GLSLDataTypeToOpenGLBaseType(GLSLDataType type)
	{
		switch (type)
		{
			case GLSLDataType::Float:    return GL_FLOAT;
			case GLSLDataType::Float2:   return GL_FLOAT;
			case GLSLDataType::Float3:   return GL_FLOAT;
			case GLSLDataType::Float4:   return GL_FLOAT;
			case GLSLDataType::Mat3:     return GL_FLOAT;
			case GLSLDataType::Mat4:     return GL_FLOAT;
			case GLSLDataType::Int:      return GL_INT;
			case GLSLDataType::Int2:     return GL_INT;
			case GLSLDataType::Int3:     return GL_INT;
			case GLSLDataType::Int4:     return GL_INT;
			case GLSLDataType::Bool:     return GL_BOOL;
		}
		BUBBLE_CORE_ASSERT(false, "Unknown GLSLDataType!");
		return 0;
	}

	VertexArray::VertexArray()
	{
		glcall(glGenVertexArrays(1, &mRendererID));
		glcall(glBindVertexArray(mRendererID));
	}

	VertexArray::VertexArray(VertexArray&& other)
	{
		mRendererID = other.mRendererID;
		VertexBufferIndex(other.mVertexBufferIndex);
		mVertexBuffers = std::move(other.mVertexBuffers);
		mIndexBuffer = std::move(other.mIndexBuffer);
		other.mRendererID = 0;
		other.VertexBufferIndex(0);
	}

	VertexArray& VertexArray::operator=(VertexArray&& other)
	{
		if (this != &other)
		{
			mRendererID = other.mRendererID;
			VertexBufferIndex(other.mVertexBufferIndex);
			mVertexBuffers = std::move(other.mVertexBuffers);
			mIndexBuffer = std::move(other.mIndexBuffer);
			other.mRendererID = 0;
			other.VertexBufferIndex(0);
		}
		return *this;
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &mRendererID);
	}

	void VertexArray::Bind() const
	{
		glcall(glBindVertexArray(mRendererID));
	}

	void VertexArray::Unbind() const
	{
		glcall(glBindVertexArray(0));
	}

	void VertexArray::AddVertexBuffer(VertexBuffer&& vertexBuffer)
	{
		BUBBLE_CORE_ASSERT(vertexBuffer.GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		Bind();
		vertexBuffer.Bind();

		const auto& layout = vertexBuffer.GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
				case GLSLDataType::Float:
				case GLSLDataType::Float2:
				case GLSLDataType::Float3:
				case GLSLDataType::Float4:
				case GLSLDataType::Int:
				case GLSLDataType::Int2:
				case GLSLDataType::Int3:
				case GLSLDataType::Int4:
				case GLSLDataType::Bool:
				{
					glcall(glEnableVertexAttribArray(mVertexBufferIndex));
					glcall(glVertexAttribPointer(mVertexBufferIndex,
												 element.GetComponentCount(),
												 GLSLDataTypeToOpenGLBaseType(element.Type),
												 element.Normalized ? GL_TRUE : GL_FALSE,
												 layout.GetStride() ? layout.GetStride() : element.Size,
												 (const void*)element.Offset));
					VertexBufferIndex(mVertexBufferIndex + 1);
				}break;
				case GLSLDataType::Mat3:
				case GLSLDataType::Mat4:
				{
					uint32_t count = element.GetComponentCount();
					for (uint32_t i = 0; i < count; i++)
					{
						glcall(glEnableVertexAttribArray(mVertexBufferIndex));
						glcall(glVertexAttribPointer(mVertexBufferIndex,
													 count,
													 GLSLDataTypeToOpenGLBaseType(element.Type),
													 element.Normalized ? GL_TRUE : GL_FALSE,
													 layout.GetStride() ? layout.GetStride() : element.Size,
													 (const void*)(sizeof(float) * count * i)));
						glcall(glVertexAttribDivisor(mVertexBufferIndex, 1));
						VertexBufferIndex(mVertexBufferIndex + 1);
					}
				}break;
				default: {
					BUBBLE_CORE_ASSERT(false, "Unknown GLSLDataType!");
				}
			}
		}
		mVertexBuffers.push_back(std::move(vertexBuffer));
		Unbind();
	}

	void VertexArray::SetIndexBuffer(IndexBuffer&& indexBuffer)
	{
		Bind();
		indexBuffer.Bind();
		mIndexBuffer = std::move(indexBuffer);
		Unbind();
	}

	std::vector<Bubble::VertexBuffer>& VertexArray::GetVertexBuffers()
	{
		return mVertexBuffers;
	}

	Bubble::IndexBuffer& VertexArray::GetIndexBuffer()
	{
		return mIndexBuffer;
	}

}
