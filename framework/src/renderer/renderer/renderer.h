#pragma once

#include "time/timer.h"
#include "renderer_base.h"
#include "vertex_array/vertex_array.h"
#include "framebuffer/framebuffer.h"
#include "shader/shader.h"
#include "buffer/buffer.h"
#include "buffer/uniform_buffer.h"

#include "glm/glm.hpp"



namespace Bubble
{
	enum class DrawType { POINTS, LINES, TRIANGLES };

	struct Renderer
	{
		static const Framebuffer* sActiveViewport;

	public:
		static void Init();

		// ============ Options ============
		static void Wareframe(bool);
		static void AlphaBlending(bool, uint32_t sfactor = GL_SRC_ALPHA, uint32_t dfactor = GL_ONE_MINUS_SRC_ALPHA);
		static void BackfaceCulling(bool);
		static void DepthTest(bool);

		// ============ Set active components ============
		static void SetViewport(const Framebuffer& framebuffer, uint32_t x = 0, uint32_t y = 0, uint32_t width = 0, uint32_t height = 0);

		// ============ Clearing ============
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void ClearDepth();
		static void ClearColor();

		// ============ Rendering ============
		static void DrawIndices(const Ref<VertexArray>& vertex_array, DrawType draw_type = DrawType::TRIANGLES, uint32_t count = 0);
		static void DrawVertices(const Ref<VertexArray>& vertex_array, DrawType draw_type = DrawType::TRIANGLES, uint32_t count = 0);
	};

}