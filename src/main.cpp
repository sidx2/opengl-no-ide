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
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

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
        -0.5,  -0.5,  0.0f, 0.0f,  // 0
         0.5,  -0.5,  1.0f, 0.0f,  // 1
         0.5,   0.5,  1.0f, 1.0f,  // 2
        -0.5,   0.5,  0.0f, 1.0f   // 3
        //  0.75, -0.5,  // 4
        //  0.5,  -1.0f, // 5
        //  1.0f, -1.0f  // 6
    };

    uint indexBuf[] = {
        0, 1, 2,
        2, 3, 0,
        // 4, 5, 6
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // uint vao;
    // glGenVertexArrays(1, &vao);
    // glBindVertexArray(vao);

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    va.Bind();
    
    // GLCall(glEnableVertexAttribArray(0));
    // GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    IndexBuffer ib(indexBuf, 9);

    // ortho/presp proj mat
    glm::mat4 proj = glm::ortho(-2.f, 2.f, -1.5f, 1.5f, -1.f, 1.f);
    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(-1.5, 0, 0));
    
    // shader
    Shader shader("./res/shaders/basic.shader");
    shader.Bind();

    float r = 0.0f;
    shader.SetUniform4f("u_Color", r, 0.3, 0.8, 1.0);

    float inc = 0.0005;

    Texture texture("./res/textures/s-logo.png");
    texture.Bind(0);
    shader.SetUniform1i("u_Texture", 0);

    va.Unbind();
    shader.Unbind();
    va.Unbind();
    ib.Unbind();

    Renderer renderer;

    ImGui::CreateContext();    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    glm::vec3 translation(0.9, 0.9, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();
        ImGui_ImplGlfwGL3_NewFrame();

        glm::mat4 model = glm::translate(glm::mat4(1.f), translation);
        glm::mat4 mvp = proj * view * model;

        
        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3, 0.8, 1.0);
        shader.SetUniformMat4f("u_MVP", mvp);

        // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        // ib.bind(); // can optionally be used to bind instead

        renderer.Draw(va, ib, shader);
        // GLCall(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f) inc = -0.0005;
        else if (r < 0.0f) inc = 0.0005f;

        r += inc;

        {                      // Display some text (you can use a format string too)
            ImGui::SliderFloat3("float", &translation.x, 0.0f, 1.0f); 
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}