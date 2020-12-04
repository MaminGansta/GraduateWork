#pragma once

#include "base.h"
#include "GL/glew.h"


#if _DEBUG
#define ASSERT(x) if (!(x)) assert(false);
#define glcall(func) GLClearError();\
        func;\
        ASSERT(GLLogCall(#func, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		LOG_CORE_ERROR("[OpenGL Error] > {} \n {} : {}", glewGetErrorString(error), file, line);
		return false;
	}
	return true;
}
#else
#define glcall(func) (func);
#endif
