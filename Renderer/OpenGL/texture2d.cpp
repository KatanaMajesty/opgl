#include "OpenGL/texture2d.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture2D::Texture2D(const std::string& path, TextureType type, bool flipV, GLint wrapParam)
    : m_type(type), m_path(path)
{
    glGenTextures(1, &m_id);

    Load(flipV, wrapParam);
}
 
Texture2D::Texture2D(Texture2D&& other)
    : m_id(other.m_id)
    , m_type(other.m_type)
    , m_path(std::move(other.m_path))
{
    other.m_id = 0;
    other.m_type = TextureType::UNDEFINED;
}

Texture2D& Texture2D::operator=(Texture2D&& other)
{
    if (this == &other)
        return *this;

    return (*this = std::move(other));
}

void Texture2D::Load(bool flipV, GLint wrapParam)
{
    stbi_set_flip_vertically_on_load(flipV);

    int32_t width;
    int32_t height;
    int32_t channels;
    unsigned char* buffer = stbi_load(m_path.c_str(), &width, &height, &channels, 0);
    if (!buffer)
    {
        std::cout << "Failed to parse texture at " << m_path << std::endl;
    }

    std::cout << "Assimp debug: Creating a texture from filepath: " << m_path << std::endl;
    std::cout << "Texture info: w" << width << ", h" << height << ", c" << channels << std::endl; 
    GLenum format = GL_NONE;
    switch (channels)
    {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: std::cout << "INCOMPATIBLE TEXTURE FORMAT!\n";
    }

    Texture2D::Bind(*this);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapParam);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    Texture2D::Unbind();

    stbi_image_free(buffer);
}