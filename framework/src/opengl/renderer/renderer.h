#pragma once

#include "time/timer.h"
#include "renderer_base.h"
#include "vertex_array/vertex_array.h"
#include "framebuffer/framebuffer.h"
#include "shader/shader.h"
#include "shader/shader_loader.h"
#include "buffer/buffer.h"
#include "buffer/uniform_buffer.h"

#include "glm/glm.hpp"



namespace Bubble
{
	enum class DrawType { POINTS, LINES, TRIANGLES };
	enum class RenderingMod { ImageProccessing, Graphic3D, Graphic2D };

	struct Renderer
	{
		static const Framebuffer* sActiveViewport;
		static RenderingMod sActiveMod;
		
		// Static data
		static Ref<VertexArray> sFullScreenVAO;
		static Ref<Shader> sPassThroughShader;

	public:
		static void Init(RenderingMod start_mode);

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

		// ============ Image operations ============
		static Ref<Texture2D> CopyTexture2D(const Ref<Texture2D>& texture);
		static Texture2D CopyTexture2D(const Texture2D& texture);
		static void DrawTexture2D(const Texture2D& src, Texture2D& dst);
		static Texture2D ResizeTexture2D(const Texture2D& src, int width, int height);

	private:
		static void LoadData();

	};

}