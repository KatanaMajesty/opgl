#pragma once

class IndexBuffer
{
private:
    GLuint m_id;

public:
    IndexBuffer();
    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;
    ~IndexBuffer() = default;

    static inline void Bind(IndexBuffer& ibo) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.m_id); }

    static inline void Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    inline void Delete() 
    { 
        if (m_id == 0)
            return;

        glDeleteBuffers(1, &m_id);
        m_id = 0;
    }

    template<typename InputIt>
    static std::enable_if_t<std::is_same_v<std::iter_value_t<InputIt>, GLuint>> 
    Data(InputIt first, InputIt last, GLenum usage) 
    {
        size_t sz = std::distance(first, last);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sz * sizeof(GLuint), std::addressof(*first), usage);
    }
};