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

#define WINDOW_WIDTH (1280.f * 0.75f)
#define WINDOW_HEIGHT (720.f * 0.75f)

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3),
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
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
        -50.f,  -50.f,  0.0f, 0.0f,  // 0
         50.f,  -50.f,  1.0f, 0.0f,  // 1
         50.f,   50.f,  1.0f, 1.0f,  // 2
        -50.f,   50.f,  0.0f, 1.0f   // 3
    };

    uint indexBuf[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    va.Bind();

    IndexBuffer ib(indexBuf, 9);

    // ortho/presp proj mat
    glm::mat4 proj = glm::ortho(0.f, WINDOW_WIDTH, 0.f, WINDOW_HEIGHT, -1.f, 1.f);
    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));
    
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

    ImGui::CreateContext();    
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    glm::vec3 translationA(100, 200, 0);
    glm::vec3 translationB(300, 200, 0);

    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        ImGui_ImplGlfwGL3_NewFrame();


        shader.Bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.f), translationB);
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }


        if (r > 1.0f) inc = -0.0005;
        else if (r < 0.0f) inc = 0.0005f;

        r += inc;

        {   // Display some text (you can use a format string too)
            ImGui::SliderFloat3("Translation A", &translationA.x, 0, 100); 
            ImGui::SliderFloat3("Translation B", &translationB.x, 0, 100); 
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