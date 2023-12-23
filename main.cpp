#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <signal.h>

#define uint unsigned int
#define ASSERT(x) if (!(x)) raise(SIGBREAK);
#define GLCall(x) \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

using namespace std;

static void GLClearError() { 
    while(glGetError()); 
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum err = glGetError()) {
        printf("ERROR:%s:%i :: %s :: (%i)\n", file, line, function, err);
        return false;
    }
    return true;
}

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

    uint buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 7 * 2 * sizeof(float), positions, GL_STATIC_DRAW));
    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    uint ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 9 * sizeof(uint), indexBuf, GL_STATIC_DRAW));

    // shaders
    shaderProgramSource src = parseShader("./res/shaders/basic.shader");

    uint shader = createShader(src.VertexSource, src.FragmentSource);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    float r = 0.0f;
    glUniform4f(location, r, 0.3, 0.8, 1.0);
    
    float inc = 0.0005;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform4f(location, r, 0.3, 0.8, 1.0);

        GLCall(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f) inc = -0.0005;
        else if (r < 0.0f) inc = 0.0005f;

        r += inc;
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}