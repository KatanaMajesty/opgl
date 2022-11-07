#pragma once

#include "Graphics/scene.h"
#include "Graphics/lighting.h"
#include "Common/camera.h"

class StencilScene : public Scene
{
private:
    glm::vec3 m_translation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_rotation = glm::vec3(-90.0f, 0.0f, 0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);

    Camera m_camera;
    
    Shader* m_defaultShader;
    Shader* m_stencilShader;
    Model* m_chair;
    
    DirectionalLight m_dirLight = {
        glm::vec3(0.7f, -0.8f, -0.6f),
        glm::vec3(0.3f, 0.28f, 0.65f),
        glm::vec3(1.0f, 0.25f, 0.2f),
        glm::vec3(1.0f, 0.85f, 1.0f),
    };

    bool m_selection = true;

public:
    StencilScene(GLFWwindow* context);
    virtual ~StencilScene() = default; 

    virtual void OnAttach() override;
    virtual void OnDetatch() override;

    virtual void Update(float timeStep) override;
    virtual void UpdateImgui(ImGuiIO& io, float timeStep) override;
};