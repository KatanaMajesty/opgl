#pragma once

#include "texture2d.h"
#include "shader.h"

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 textureUv;
};

class Mesh
{
private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<Texture2D> m_textures;

public:
    Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, std::vector<Texture2D>&& textures);

    void render(Shader& shader, const std::string& uniform);
};