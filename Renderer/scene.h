#pragma once

#include "model.h"
#include "shader.h"
#include "camera.h"

class Scene
{
private:
    std::vector<Model*> m_modelBuffer;
    std::vector<Shader*> m_shaderBuffer;

protected:
    GLFWwindow* m_context;

public:
    Scene(GLFWwindow* context);
    virtual ~Scene();
    
    Model* CreateModel(const std::string& path, bool loadTextures = true, bool flipV = false);

    Shader* CreateShader(const std::string& vert, const std::string& frag);

    inline virtual void Update(float timeStep) {}
    
    inline virtual void UpdateImgui(ImGuiIO& io, float timeStep) {}
};