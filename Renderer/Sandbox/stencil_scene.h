#pragma once

#include "Graphics/scene.h"
#include "Common/camera.h"

class StencilScene : public Scene
{
private:
    glm::vec3 m_translation;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;    

    Camera m_camera;
    
    Shader* m_stencilShader;
    
public:
    StencilScene(GLFWwindow* context);
    virtual ~StencilScene() = default; 

    virtual void Update(float timeStep) override;

    virtual void UpdateImgui(ImGuiIO& io, float timeStep) override;
};