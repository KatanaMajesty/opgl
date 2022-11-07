#pragma once

#include "Graphics/model.h"
#include "OpenGL/shader.h"
#include "Common/camera.h"

class Scene
{
private:
    std::vector<Model*> m_modelBuffer;
    std::vector<Shader*> m_shaderBuffer;

protected:
    GLFWwindow* m_context;
    std::string m_name;

public:
    Scene(GLFWwindow* context, const std::string& name);
    virtual ~Scene();
    
    Model* CreateModel(const std::string& path, bool loadTextures = true, bool flipV = false);
    Shader* CreateShader(const std::string& vert, const std::string& frag);

    inline virtual void OnAttach() { std::cout << "Scene: Attaching " << m_name << " scene!\n"; };
    inline virtual void OnDetatch() { std::cout << "Scene: Detatching " << m_name << " scene!\n"; };

    inline virtual void Update(float timeStep) {}
    inline virtual void UpdateImgui(ImGuiIO& io, float timeStep) {}

    inline constexpr const std::string& GetName() const { return m_name; }
};