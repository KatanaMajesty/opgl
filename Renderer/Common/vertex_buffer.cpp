#include "Common/vertex_buffer.h"

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_id);
}