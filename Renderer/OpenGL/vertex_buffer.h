#pragma once

class VertexBuffer
{
private:
    GLuint m_id;

public:
    VertexBuffer();

    static inline void Bind(VertexBuffer& vbo) { glBindBuffer(GL_ARRAY_BUFFER, vbo.m_id); }

    static inline void Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    template<typename InputIt>
    static void Data(InputIt first, InputIt last, GLenum usage)
    {
        size_t sz = std::distance(first, last);
        glBufferData(GL_ARRAY_BUFFER, sz * sizeof(std::iter_value_t<InputIt>), std::addressof(*first), usage);
    }
};