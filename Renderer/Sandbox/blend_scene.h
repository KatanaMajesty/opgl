#pragma once

#include "Graphics/scene.h"
#include "Common/camera.h"

class BlendScene : public Scene
{
private:
    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_projection;

    Camera m_camera;

    Shader* m_depthShader;

public:
    BlendScene(GLFWwindow* context);

    virtual void Update(float timeStep) override;
    
    virtual void UpdateImgui(ImGuiIO& io, float timeStep) override;
};