#pragma once

#include "Graphics/scene.h"
#include "Common/camera.h"

class Window;

class RenderManager
{
public:
    using value_type = Scene;
    using container_type = std::vector<value_type*>;

private:
    RenderManager(GLFWwindow* context);

public:
    ~RenderManager();

    template<typename T, typename... Args>
    T* RegisterScene(Args&&... args)
    {
        Scene* scene = m_scenes.emplace_back(new T(std::forward<Args>(args)...));
        if (m_scenes.size() == 1)
        {
            scene->OnAttach();
            m_currentScene = scene;
        }
            
        return (T*) scene;
    }

    void Update(float timeStep);
    void UpdateImgui(ImGuiIO& io, float timeStep);

    inline container_type& GetScenes() { return m_scenes; }
    
    inline void SetCurrentScene(Scene* scene);
    inline value_type* GetCurrentScene() { return m_currentScene; }

    inline void SetMenuEnabled(bool v) { m_menuEnabled = v; } 
    inline bool IsMenuEnabled() const { return m_menuEnabled; }

private:
    friend class Window; // in order to access the private constructor

    container_type m_scenes; // as we support polymorphism
    value_type* m_currentScene = nullptr;
    bool m_menuEnabled = false;
    GLFWwindow* m_context;
};