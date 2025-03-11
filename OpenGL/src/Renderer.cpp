#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError()); // When there's no more errors, glGetError returns a GL_NO_ERROR flag that's equal to 0
}

bool GLLogCall(const char* function, const char* file, const int& line)
{
    bool anyErrors = false;
    while (GLenum e = glGetError()) // This will loop until e = 0 or false
    {
        std::cout << "[OpenGL Error] (" << e << ") " << function <<
            " " << file << ":" << line << std::endl;
        anyErrors = true;
    }
    return anyErrors;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind(); // Bind Vertex Array

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}