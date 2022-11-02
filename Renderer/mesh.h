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
    std::vector<Texture2D> m_diffuseTextures;
    std::vector<Texture2D> m_specularTextures;

    // temporary
    float m_shininess;

public:
    Mesh(std::vector<Vertex>&& vertices
        , std::vector<GLuint>&& indices
        , std::vector<Texture2D>&& diffuseTextures
        , std::vector<Texture2D>&& specularTextures
        , float shininess);

    void Render(Shader& shader, const std::string& uniform);

private:
    void BindDiffuses(Shader& shader, const std::string& uniform);

    void BindSpeculars(Shader& shader, const std::string& uniform);

    void Unbind();
};