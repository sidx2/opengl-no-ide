#include "Renderer.h"
#include <stdio.h>
#include <gl/glew.h>

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