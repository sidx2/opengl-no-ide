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

#include "tests/Test.h"
#include "tests/TestClearColor.h"

#define uint unsigned int

using namespace std;

#define WINDOW_WIDTH (1280.f * 0.75f)
#define WINDOW_HEIGHT (720.f * 0.75f)

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3),
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!glewInit() == GLEW_OK) {
        std::cout << "Glew init failed!\n";
    };

    cout << "version: " << string((const char *) glGetString(GL_VERSION)) << endl;

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Renderer renderer;

    ImGui::CreateContext();    
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    test::Test* currentTest = nullptr;
    test::TestMenu* menu = new test::TestMenu(currentTest);
    currentTest = menu;

    menu -> RegisterTest<test::TestClearColor>("Clear Color");
    
    while (!glfwWindowShouldClose(window)) {
        renderer.Clear();

        ImGui_ImplGlfwGL3_NewFrame();  

        if (currentTest) {
            currentTest -> OnUpdate(0.f);
            currentTest -> OnRender();
            ImGui::Begin("Test");
            if (currentTest != menu && ImGui::Button("<-")) {
                delete currentTest;
                currentTest = menu;
            }
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}