#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <signal.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"

#define uint unsigned int

using namespace std;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3),
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (!glewInit() == GLEW_OK) {
        std::cout << "Glew init failed!\n";
    };

    cout << "version: " << string((const char *) glGetString(GL_VERSION)) << endl;

    float positions[] = {
        -0.5,  -0.5,  // 0
         0.5,  -0.5,  // 1
         0.5,   0.5,  // 2
        -0.5,   0.5,  // 3
         0.75, -0.5,  // 4
         0.5,  -1.0f, // 5
         1.0f, -1.0f  // 6
    };

    uint indexBuf[] = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6
    };

    // uint vao;
    // glGenVertexArrays(1, &vao);
    // glBindVertexArray(vao);

    VertexArray va;
    VertexBuffer vb(positions, 7 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    va.Bind();
    
    // GLCall(glEnableVertexAttribArray(0));
    // GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    IndexBuffer ib(indexBuf, 9);

    // shader
    Shader shader("./res/shaders/basic.shader");
    shader.Bind();

    float r = 0.0f;
    shader.SetUniform4f("u_Color", r, 0.3, 0.8, 1.0);

    float inc = 0.0005;

    va.Unbind();
    shader.Unbind();
    va.Unbind();
    ib.Unbind();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3, 0.8, 1.0);

        va.Bind();

        // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        // ib.bind(); // can optionally be used to bind instead

        GLCall(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f) inc = -0.0005;
        else if (r < 0.0f) inc = 0.0005f;

        r += inc;
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}