#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, std::vector<Texture2D>&& textures)
    : m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_textures(std::move(textures))
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

void Mesh::render(Shader& shader, const std::string& uniform)
{
    GLuint diffuseNum = 1;
    GLuint specularNum = 1;
    for (GLuint i = 0; i < m_textures.size(); i++)
    {
        Texture2D& texture = m_textures[i];
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture.m_id);
        bool diffuse = texture.m_type == TextureType::Diffuse;
        std::string s = uniform + (diffuse ? ".t_diffuse" : ".t_specular") + std::to_string((diffuse ? diffuseNum++ : specularNum++));
        shader.bind();
        shader.setUniform1i(s.c_str(), i);
    }
    
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}