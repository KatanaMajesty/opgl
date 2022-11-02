#pragma once

#include "texture2d.h"
#include "shader.h"
#include "Common/index_buffer.h"
#include "Common/vertex_buffer.h"
#include "Common/vertex_array.h"

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 textureUv;
};

class Mesh
{
private:

    VertexArray m_vao;
    VertexBuffer m_vbo;
    IndexBuffer m_ibo;
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<Texture2D> m_diffuseTextures;
    std::vector<Texture2D> m_specularTextures;
    Material m_material;

public:
    Mesh(std::vector<Vertex>&& vertices
        , std::vector<GLuint>&& indices
        , std::vector<Texture2D>&& diffuseTextures
        , std::vector<Texture2D>&& specularTextures
        , const Material& material);

    void Render(Shader& shader, const std::string& uniform);

private:
    void BindDiffuses(Shader& shader, const std::string& uniform);

    void BindSpeculars(Shader& shader, const std::string& uniform);

    void BindMaterial(Shader& shader, const std::string& uniform);

    void Unbind();
};