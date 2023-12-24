#define GLEW_STATIC
#include "Shader.h"
#include <gl/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <signal.h>
#include "Renderer.h"

Shader::Shader(const std::string &filepath)
    : m_Filepath(filepath), m_RendererID(0) {
    shaderProgramSource src = ParseShader(m_Filepath);
    m_RendererID = CreateShader(src.VertexSource, src.FragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

shaderProgramSource Shader::ParseShader(const std::string &filepath) {
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];

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

unsigned int Shader::CompileShader(unsigned int type, const std::string &source) {
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

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string &name, int value) {
    unsigned int location = GetUniformLocation(name);
    glUniform1i(location, value);
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    unsigned int location = GetUniformLocation(name);
    glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &mat) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

unsigned int Shader::GetUniformLocation(const std::string &name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) 
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: Uniform `" << name << "` doesn't exist\n";

    m_UniformLocationCache[name] = location;
    return location;
}