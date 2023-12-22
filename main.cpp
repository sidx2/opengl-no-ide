#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#define uint unsigned int
using namespace std;

struct shaderProgramSource {
    string VertexSource;
    string FragmentSource;
};

static shaderProgramSource parseShader(const string &filepath) {
    ifstream stream(filepath);
    string line;
    stringstream ss[2];

    enum class shaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    shaderType type = shaderType :: NONE;

    while(getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                // set mode to vertex
                type = shaderType :: VERTEX;
            }

            else if (line.find("fragment") != std::string::npos) {
                // set mode to fragment
                type = shaderType :: FRAGMENT;
            }
        }

        else ss[(int) type] << line << "\n";
    }

    return {ss[0].str(), ss[1].str()};
}

static unsigned int compileShader(unsigned int type, const string &source) {
    unsigned int id = glCreateShader(type);
    const char* str = source.c_str();

    glShaderSource(id, 1, &str, nullptr);
    glCompileShader(id);

    // error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char *buffer = (char*) alloca(len * sizeof(char));
        glGetShaderInfoLog(id, len, &len, buffer);

        printf("ERROR: failed to compile %s shader: %s", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), buffer);
        
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShader(const string &vertexShader, const string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

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

    float positions[] = {
        -0.5, -0.5, // 0
         0.5, -0.5, // 1
         0.5,  0.5, // 2
        -0.5,  0.5  // 3
    };

    uint indexBuf[] = {
        0, 1, 2,
        2, 3, 0
    };

    uint a;
    glGenBuffers(1, &a);
    glBindBuffer(GL_ARRAY_BUFFER, a);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    uint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint), indexBuf, GL_STATIC_DRAW);
    // shaders
    shaderProgramSource src = parseShader("./res/shaders/basic.shader");

    uint shader = createShader(src.VertexSource, src.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}