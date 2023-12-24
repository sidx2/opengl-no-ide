#pragma once
#define GLEW_STATIC
#include <gl/glew.h>
#include <signal.h>
#include "VertexArray.h"
#include "IndexBuffer.h"

class Shader;

#define ASSERT(x) if (!(x)) raise(SIGBREAK);
#define GLCall(x) \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
    public:
        void Clear() const;
        void Draw(const VertexArray &va, const IndexBuffer &ib, Shader &shader) const;
};