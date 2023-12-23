#pragma once

class IndexBuffer {
    private:
        unsigned int m_RendererID;
        unsigned int m_Count;
    public:
        IndexBuffer(const unsigned int* data, int count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline unsigned int GetCount() { return m_Count; }
};