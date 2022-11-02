#include "mesh.h"

#define RENDERER_MAX_DIFFUSES 4
#define RENDERER_MAX_SPECULARS 4

Mesh::Mesh(std::vector<Vertex>&& vertices
        , std::vector<GLuint>&& indices
        , std::vector<Texture2D>&& diffuseTextures
        , std::vector<Texture2D>&& specularTextures
        , float shininess)
    : m_vertices(std::move(vertices))
    , m_indices(std::move(indices))
    , m_diffuseTextures(std::move(diffuseTextures))
    , m_specularTextures(std::move(specularTextures))
    , m_shininess(shininess)
{
    glCreateVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (offsetof(Vertex, pos)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (offsetof(Vertex, normal)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (offsetof(Vertex, textureUv)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::Render(Shader& shader, const std::string& uniform)
{
    GLuint diffuseNum = 1;
    GLuint specularNum = 1;

    // Unbind(shader, uniform);
    BindDiffuses(shader, uniform);
    BindSpeculars(shader, uniform);
    
    shader.bind();
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::BindDiffuses(Shader& shader, const std::string& uniform)
{
    shader.bind();

    for (size_t i = 0; i < m_diffuseTextures.size() && i < RENDERER_MAX_DIFFUSES; i++)
    {
        Texture2D& texture = m_diffuseTextures[i];
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture.m_id);
        shader.setUniform1i(uniform + ".t_diffuse" + std::to_string(i + 1), i);
    }
}

void Mesh::BindSpeculars(Shader& shader, const std::string& uniform)
{
    shader.bind();

    size_t diffSize = m_diffuseTextures.size();
    bool useDiffuses = m_specularTextures.size() == 0;
    size_t iterations = useDiffuses ? diffSize : m_specularTextures.size();
    const std::vector<Texture2D>& buffer = useDiffuses ? m_diffuseTextures : m_specularTextures;

    for (size_t i = 0; i < iterations && i < RENDERER_MAX_SPECULARS; i++)
    {
        const Texture2D& texture = buffer.at(i);
        glActiveTexture(GL_TEXTURE0 + i + diffSize);
        glBindTexture(GL_TEXTURE_2D, texture.m_id);
        shader.setUniform1i(uniform + ".t_specular" + std::to_string(i + 1), i + diffSize);
    }
    // Move this to method BindMaterial, when implemented
    shader.setUniform1f(uniform + ".shininess", m_shininess);
}

void Mesh::Unbind()
{
    for (size_t i = 0; i < RENDERER_MAX_DIFFUSES + RENDERER_MAX_SPECULARS; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glActiveTexture(GL_TEXTURE0);
}