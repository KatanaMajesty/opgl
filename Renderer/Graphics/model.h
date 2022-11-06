#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OpenGL/shader.h"
#include "Graphics/mesh.h"

class Model
{
private:
    std::unordered_map<std::string, Texture2D> m_textureCache;
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    bool m_loadTextures;
    bool m_flipV;
    uint32_t m_vertexCount = 0;
    uint32_t m_faceCount = 0;

public:
    Model(const std::string& path, bool loadTextures, bool flipV);
    ~Model();

    void Render(Shader& shader, const std::string& uniform);

    inline constexpr uint32_t GetVertexCount() const { return m_vertexCount; } 

    inline constexpr uint32_t GetFaceCount() const { return m_faceCount; } 

private:
    void Load(const std::string& path);
    
    void ProcessNode(aiNode* node, const aiScene* scene);
    
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    
    std::vector<Texture2D*> ProcessMaterialTextures(aiMaterial* material, aiTextureType type, TextureType ourType);
};