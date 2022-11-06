#include "Graphics/mesh.h"

#define RENDERER_MAX_DIFFUSES 4
#define RENDERER_MAX_SPECULARS 4

Mesh::Mesh(std::vector<Vertex>&& vertices
        , std::vector<GLuint>&& indices
        , std::vector<Texture2D*>&& diffuseTextures
        , std::vector<Texture2D*>&& specularTextures
        , const Material& material)
    : m_vertices(std::move(vertices))
    , m_indices(std::move(indices))
    , m_diffuseTextures(std::move(diffuseTextures))
    , m_specularTextures(std::move(specularTextures))
    , m_material(material)
{
    VertexArray::Bind(m_vao);

    VertexBuffer::Bind(m_vbo);
    VertexBuffer::Data(m_vertices.begin(), m_vertices.end(), GL_STATIC_DRAW);

    IndexBuffer::Bind(m_ibo);
    IndexBuffer::Data(m_indices.begin(), m_indices.end(), GL_STATIC_DRAW);

    // This shouldn't ever be done before VertexBuffer::Bind to currently used VAO
    VertexArray::AddLayout(m_vao, 
    {
        VBLayout{3, GL_FLOAT},
        VBLayout{3, GL_FLOAT},
        VBLayout{2, GL_FLOAT}
    });

    VertexArray::Unbind();
}

Mesh::~Mesh()
{
    m_ibo.Delete();
    m_vbo.Delete();
    m_vao.Delete();
}

void Mesh::Render(Shader& shader, const std::string& uniform)
{
    Unbind();
    BindDiffuses(shader, uniform);
    BindSpeculars(shader, uniform);
    BindMaterial(shader, uniform);
    
    VertexArray::Bind(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    VertexArray::Unbind();

    Shader::Unbind();
}

void Mesh::BindDiffuses(Shader& shader, const std::string& uniform)
{
    for (size_t i = 0; i < m_diffuseTextures.size() && i < RENDERER_MAX_DIFFUSES; i++)
    {
        Texture2D* texture = m_diffuseTextures[i];
        Texture2D::Bind(*texture, i);
        shader.SetUniform1i(uniform + ".t_diffuse" + std::to_string(i + 1), i);
    }
}

void Mesh::BindSpeculars(Shader& shader, const std::string& uniform)
{
    size_t diffSize = m_diffuseTextures.size();
    bool useDiffuses = m_specularTextures.size() == 0;
    size_t iterations = useDiffuses ? diffSize : m_specularTextures.size();
    const std::vector<Texture2D*>& buffer = useDiffuses ? m_diffuseTextures : m_specularTextures;

    for (size_t i = 0; i < iterations && i < RENDERER_MAX_SPECULARS; i++)
    {
        const Texture2D* texture = buffer.at(i);
        Texture2D::Bind(*texture, i + diffSize);
        shader.SetUniform1i(uniform + ".t_specular" + std::to_string(i + 1), i + diffSize);
    }
}

void Mesh::BindMaterial(Shader& shader, const std::string& uniform)
{
    shader.SetUniformMaterial(uniform, m_material);
}

void Mesh::Unbind()
{
    for (size_t i = 0; i < RENDERER_MAX_DIFFUSES + RENDERER_MAX_SPECULARS; i++)
        Texture2D::Unbind(i);

    glActiveTexture(GL_TEXTURE0);
}