#pragma once

#include "model.h"
#include "shader.h"
#include "camera.h"

class Scene
{
private:
    std::vector<Model> m_modelBuffer;
    std::vector<Shader> m_shaderBuffer;

protected:
    GLFWwindow* m_context;
    Camera& m_camera;

public:
    Scene(GLFWwindow* context, Camera& camera)
        : m_context(context), m_camera(camera)
    {   
    }
    virtual ~Scene() = default;
    
    template<typename... Args>
    Model* createModel(Args&&... args)
    {
        m_modelBuffer.emplace_back(std::forward<Args>(args)...);
        std::cout << "Was here!\n";
        return &m_modelBuffer.back();
    }

    template<typename... Args>
    Shader* createShader(Args&&... args)
    {
        m_shaderBuffer.emplace_back(std::forward<Args>(args)...);
        return &m_shaderBuffer.back();
    }

    inline virtual void update(float timeStep) {}
};