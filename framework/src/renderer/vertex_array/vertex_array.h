#pragma once

#include "buffer/buffer.h"

namespace Bubble
{
	class VertexArray
	{
		uint32_t mRendererID;
		uint32_t mVertexBufferIndex = 0;
		std::vector<VertexBuffer> mVertexBuffers;
		IndexBuffer mIndexBuffer;

	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		VertexArray(VertexArray&&);
		VertexArray& operator=(VertexArray&&);


		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(VertexBuffer&& vertexBuffer);
		void SetIndexBuffer(IndexBuffer&& indexBuffer);

		const uint32_t GetID() const { return mRendererID; }
		std::vector<VertexBuffer>& GetVertexBuffers();
		IndexBuffer& GetIndexBuffer();
		void VertexBufferIndex(uint32_t val) { mVertexBufferIndex = val; }
	};

}
