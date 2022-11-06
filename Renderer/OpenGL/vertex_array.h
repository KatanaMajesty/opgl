#pragma once

struct VBLayout
{
    int32_t count;
    GLenum type;
    bool normalized = false;
};

class VertexArray
{
private:
    GLuint m_id;
    size_t m_layoutIdx;
    size_t m_stride;

public:
    VertexArray();
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;
    ~VertexArray() = default;

    static inline void Bind(VertexArray& vao) { glBindVertexArray(vao.m_id); }

    static inline void Unbind() { glBindVertexArray(0); }

    inline void Delete() 
    { 
        if (m_id == 0)
            return;
            
        glDeleteVertexArrays(1, &m_id);
        m_id = 0;
    }

    static void AddLayout(VertexArray& vao, const std::vector<VBLayout>& layouts);
};