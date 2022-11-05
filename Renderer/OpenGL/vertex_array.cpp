#include "OpenGL/vertex_array.h"

VertexArray::VertexArray()
    : m_layoutIdx(0)
    , m_stride(0)
{
    glCreateVertexArrays(1, &m_id);
}

void VertexArray::AddLayout(VertexArray& vao, const std::vector<VBLayout>& layouts)
{
    static const std::unordered_map<GLenum, size_t> sizeMap = {
        {GL_FLOAT, sizeof(float)},
        {GL_UNSIGNED_INT, sizeof(uint32_t)}
    };

    // we count stride here... that's all we do in this loop    
    for (const VBLayout& layout : layouts)
    {
        size_t sz = 0;
        auto it = sizeMap.find(layout.type);
        if (it != sizeMap.end())
            sz = it->second;

        vao.m_stride += sz * layout.count;
    }

    // another loop to actually bind, activate and count offsets of attributes
    size_t offset = 0;
    for (const VBLayout& layout : layouts)
    {
        int32_t sz = 0;
        auto it = sizeMap.find(layout.type);
        if (it != sizeMap.end())
            sz = static_cast<int32_t>(it->second);

        glVertexAttribPointer(vao.m_layoutIdx, layout.count, layout.type, layout.normalized, vao.m_stride, (void*) offset);
        glEnableVertexAttribArray(vao.m_layoutIdx);
        vao.m_layoutIdx++;
        offset += sz * layout.count;
    }
}