#pragma once

#include "OpenGL/texture2d.h"
#include "OpenGL/shader.h"
#include "OpenGL/index_buffer.h"
#include "OpenGL/vertex_buffer.h"
#include "OpenGL/vertex_array.h"

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
    std::vector<Texture2D*> m_diffuseTextures;
    std::vector<Texture2D*> m_specularTextures;
    Material m_material;

public:
    Mesh(std::vector<Vertex>&& vertices
        , std::vector<GLuint>&& indices
        , std::vector<Texture2D*>&& diffuseTextures
        , std::vector<Texture2D*>&& specularTextures
        , const Material& material);
    ~Mesh();
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh(Mesh&& other) = default;
    Mesh& operator=(Mesh&& other) = default;

    void Render(Shader& shader, const std::string& uniform);

private:
    void BindDiffuses(Shader& shader, const std::string& uniform);
    void BindSpeculars(Shader& shader, const std::string& uniform);
    void BindMaterial(Shader& shader, const std::string& uniform);

    void Unbind();
};