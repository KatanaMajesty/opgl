#include "OpenGL/vertex_buffer.h"

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_id);
}
    
VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0; // Important part of move constructor
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if (this == &other)
        return *this;

    return (*this = std::move(other));
}