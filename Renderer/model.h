#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"

class Model
{
private:
    std::vector<Texture2D> m_textureCache;
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    bool m_flipV;
    uint32_t m_vertexCount = 0;
    uint32_t m_faceCount = 0;

public:
    Model(const std::string& path, bool flipV = false);
    ~Model() = default;

    void render(Shader& shader, const std::string& uniform);

    inline constexpr uint32_t getVertexCount() const { return m_vertexCount; } 
    inline constexpr uint32_t getFaceCount() const { return m_faceCount; } 

private:
    void load(const std::string& path);    
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture2D> processMaterialTextures(aiMaterial* material, aiTextureType type, TextureType ourType);
};