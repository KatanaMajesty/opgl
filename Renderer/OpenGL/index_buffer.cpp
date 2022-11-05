#include "OpenGL/index_buffer.h"

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &m_id);
}