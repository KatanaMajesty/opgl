#include "Graphics/model.h"


Model::Model(const std::string& path, bool loadTextures, bool flipV)
    : m_loadTextures(loadTextures)
    , m_flipV(flipV)
{
    Load(path);
}

Model::~Model()
{
    for (auto& [_, texture] : m_textureCache)
        texture.Delete();
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
        m_meshes.push_back(std::move(ProcessMesh(mesh, scene)));
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
    std::vector<Texture2D*> specularTextures;
    std::vector<Texture2D*> diffuseTextures;
    Material material;

    m_vertexCount = mesh->mNumVertices;
    for (size_t i = 0; i < m_vertexCount; i++)
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

        vertices.push_back(vertex);
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
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

        mat->Get(AI_MATKEY_COLOR_AMBIENT, material.ambient);
        mat->Get(AI_MATKEY_COLOR_DIFFUSE, material.diffuse);
        mat->Get(AI_MATKEY_COLOR_SPECULAR, material.specular);
        mat->Get(AI_MATKEY_SHININESS, material.shininess);

        diffuseTextures = ProcessMaterialTextures(mat, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
        specularTextures = ProcessMaterialTextures(mat, aiTextureType_SPECULAR, TextureType::SPECULAR);
    }

    return Mesh(std::move(vertices), std::move(indices), std::move(diffuseTextures), std::move(specularTextures), material);
}

std::vector<Texture2D*> Model::ProcessMaterialTextures(aiMaterial* material, aiTextureType type, TextureType ourType)
{
    if (!m_loadTextures)
        return {};
        
    std::vector<Texture2D*> textures;
    
    for (size_t i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);

        std::string texturePath(m_directory + '/' + str.C_Str());
        auto it = m_textureCache.find(texturePath);

        if (it != m_textureCache.end())
        {
            textures.emplace_back(std::addressof(it->second));
        }
        else 
        {
            // try emplace just to ensure we're not adding unnecessary
            auto [iter, _] = m_textureCache.try_emplace(texturePath, Texture2D(texturePath, ourType, m_flipV));
            textures.emplace_back(std::addressof(iter->second));
        }
    }

    return textures;
}
