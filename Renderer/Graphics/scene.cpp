#include "Graphics/scene.h"

Scene::Scene(GLFWwindow* context, const std::string& name)
    : m_context(context), m_name(name)
{
}

Scene::~Scene()
{
    for (Model* model : m_modelBuffer)
        delete model;

    for (Shader* shader : m_shaderBuffer)
        delete shader;
}
    
Model* Scene::CreateModel(const std::string& path, bool loadTextures, bool flipV)
{
    return m_modelBuffer.emplace_back(new Model(path, loadTextures, flipV));
}

Shader* Scene::CreateShader(const std::string& vert, const std::string& frag)
{
    return m_shaderBuffer.emplace_back(new Shader(vert, frag));
}