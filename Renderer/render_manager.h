#pragma once

#include "scene.h"
#include "camera.h"

class Window;

// Prbbly, rewrite whole RenderManager class, remove singleton pattern
class RenderManager
{
public:

private:
    RenderManager(GLFWwindow* context);

public:
    ~RenderManager();

    template<typename T, typename... Args>
    T* AddScene(Args&&... args)
    {
        return (T*) m_scenes.emplace_back(new T(std::forward<Args>(args)...));
    }

    void Update(float timeStep);

    void UpdateImgui(ImGuiIO& io, float timeStep);

    inline std::vector<Scene*>& GetScenes() { return m_scenes; }

private:
    friend class Window;

    std::vector<Scene*> m_scenes; // as we support polymorphism
    GLFWwindow* m_context;
};