#include "OpenGL/index_buffer.h"

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &m_id);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0; // Important part of move constructor
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    if (this == &other)
        return *this;

    return (*this = std::move(other));
}