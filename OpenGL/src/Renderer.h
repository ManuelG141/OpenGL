#pragma once

#include <GL/glew.h>
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#ifdef PR_DEBUG // If we're debugging, we'll use all Log and error checking functions
#define ASSERT(x) if (x) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else // If we're on release mode, we won't define them
#define ASSERT(x)
#define GLCall(x) x
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, const int& line);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};