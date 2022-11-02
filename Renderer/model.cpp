#include "model.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Model::Model(const std::string& path, bool flipV)
    : m_flipV(flipV)
{
    Load(path);
}

void Model::Render(Shader& shader, const std::string& uniform)
{
    for (Mesh& mesh : m_meshes)
        mesh.Render(shader, uniform);
}

void Model::Load(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    std::cout << "Assimp: loading model " << path << std::endl;

    if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
    {
        std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
        return;
    }
    m_directory = path.substr(0, path.find_last_of('/'));
    std::cout << "Assimp: Model directory is: " << m_directory << std::endl;

    ProcessNode(scene->mRootNode, scene);
    std::cout << "Assimp: Model " << path << " was successfully loaded!\n";
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(ProcessMesh(mesh, scene));
    }
    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture2D> specularTextures;
    std::vector<Texture2D> diffuseTextures;
    float shininess;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        aiVector3D pos = mesh->mVertices[i];
        vertex.pos = glm::vec3(pos.x, pos.y, pos.z);

        vertex.normal = glm::vec3(0.0f);
        if (mesh->HasNormals())
        {
            aiVector3D norm = mesh->mNormals[i];
            vertex.normal = glm::vec3(norm.x, norm.y, norm.z);
        }

        aiVector3D tex(0.0f);
        if (mesh->mTextureCoords[0])
        {
            tex = mesh->mTextureCoords[0][i];
        }
        vertex.textureUv = glm::vec2(tex.x, tex.y);
        // std::cout << "texture uv!: " << vertex.textureUv.x << ", " << vertex.textureUv.y << std::endl;

        vertices.push_back(vertex);
        m_vertexCount++;
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        ++m_faceCount;
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        if (material->Get(AI_MATKEY_SHININESS, shininess) != AI_SUCCESS)
        {
            std::cout << "Failed to parse shininess in model " << m_directory << std::endl;
        }

        diffuseTextures = ProcessMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
        specularTextures = ProcessMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
    }

    return Mesh(std::move(vertices), std::move(indices), std::move(diffuseTextures), std::move(specularTextures), shininess);
}

std::vector<Texture2D> Model::ProcessMaterialTextures(aiMaterial* material, aiTextureType type, TextureType ourType)
{
    std::vector<Texture2D> textures;
    
    for (size_t i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);

        std::string texturePath(m_directory + '/' + str.C_Str());
        auto it = std::find_if(m_textureCache.begin(), m_textureCache.end(), [&](Texture2D& t) { return t.m_path == texturePath; });

        if (it != m_textureCache.end())
        {
            textures.push_back(*it);
        } else 
        {
            Texture2D texture;
            texture.m_path = std::move(texturePath);
            texture.m_type = ourType;

            stbi_set_flip_vertically_on_load(m_flipV);
            int32_t width;
            int32_t height;
            int32_t channels;
            unsigned char* buffer = stbi_load(texture.m_path.c_str(), &width, &height, &channels, 0);
            if (!buffer)
            {
                std::cout << "Failed to parse texture " << texture.m_path << std::endl;
            }
            std::cout << "Assimp debug: Creating a texture from filepath: " << texture.m_path << std::endl;
            std::cout << "Texture info: w" << width << ", h" << height << ", c" << channels << std::endl; 
            GLenum format = GL_NONE;
            switch (channels)
            {
                case 1: format = GL_RED; break;
                case 3: format = GL_RGB; break;
                case 4: format = GL_RGBA; break;
                default: std::cout << "INCOMPATIBLE TEXTURE FORMAT!\n";
            }

            glGenTextures(1, &texture.m_id);
            glBindTexture(GL_TEXTURE_2D, texture.m_id);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer);
            glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(buffer);
            stbi_set_flip_vertically_on_load(false);

            textures.push_back(texture);
            m_textureCache.push_back(texture);
        }
    }

    return textures;
}
