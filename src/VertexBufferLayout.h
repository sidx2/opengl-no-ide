#pragma once
#define GLEW_STATIC
#include <vector>
#include "Renderer.h"
#include <signal.h>
#include <gl/GL.h>

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout {
    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;
    public:
        VertexBufferLayout() : m_Stride(0) {};

        template<typename T>
        void Push(unsigned int count);

        // template<>
        // void Push<float>(unsigned int count) {
        //     m_Elements.push_back({ GL_FLOAT, count, GL_FALSE});
        //     m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
        // }

        // template<>
        // void Push<unsigned int>(unsigned int count) {
        //     m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        //     m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
        // }

        // template<>
        // void Push<unsigned char>(unsigned int count) {
        //     m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        //     m_Stride += count * VertexBufferElement::GetSizeOfType(GL_BYTE);
        // }

        inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
        inline const unsigned int GetStride() const { return m_Stride; }
};


// Move template specializations outside the class declaration
template<>
void VertexBufferLayout::Push<float>(unsigned int count);

template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count);

template<>
void VertexBufferLayout::Push<unsigned char>(unsigned int count);