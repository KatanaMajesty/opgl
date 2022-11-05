#pragma once

#include "Graphics/scene.h"
#include "Graphics/lighting.h"

class ModelScene : public Scene
{
private:
    Shader* m_shader;
    Shader* m_depthShader;
    Shader* m_stencilShader;
    std::unordered_map<std::string, Model*> m_models;

    Camera m_camera;

    glm::vec3 m_translation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f);

    DirectionalLight m_dirLight = {
        glm::vec3(0.7f, -0.8f, -0.6f),
        glm::vec3(0.3f, 0.28f, 0.65f),
        glm::vec3(1.0f, 0.25f, 0.2f),
        glm::vec3(1.0f, 0.85f, 1.0f),
    };

    bool m_enableLights = true;

public:
    ModelScene(GLFWwindow* context);

    virtual void Update(float timeStep) override;

    virtual void UpdateImgui(ImGuiIO& io, float timeStep) override;

    inline void AddModel(const std::string& name, Model* model) { m_models[name] = model; }
};