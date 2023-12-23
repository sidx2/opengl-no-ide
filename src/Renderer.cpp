#include "Renderer.h"
#include <stdio.h>
#include <gl/glew.h>
#include <signal.h>

#include "Shader.h"

void GLClearError() { 
    while(glGetError()); 
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum err = glGetError()) {
        printf("ERROR:%s:%i :: %s :: (%i)\n", file, line, function, err);
        return false;
    }
    return true;
}

void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, Shader &shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr));
}