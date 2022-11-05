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

    static inline void Bind(VertexArray& vao) { glBindVertexArray(vao.m_id); }

    static inline void Unbind() { glBindVertexArray(0); }

    static inline void Delete(VertexArray& vao) { glDeleteVertexArrays(1, &vao.m_id); }

    static void AddLayout(VertexArray& vao, const std::vector<VBLayout>& layouts);
};