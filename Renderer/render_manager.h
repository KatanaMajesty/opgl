#pragma once

#include "scene.h"
#include "camera.h"

class RenderManager
{
private:
    std::vector<std::unique_ptr<Scene>> m_scenes; // as we support polymorphism
    GLFWwindow* m_context;
    Camera m_camera;

public:
    RenderManager(GLFWwindow* context);
    ~RenderManager() = default;

    template<typename T, typename... Args>
    T* addScene(Args&&... args)
    {
        m_scenes.emplace_back(std::make_unique<T>(std::forward<Args>(args)..., m_camera));
        return (T*) m_scenes.back().get();
    }

    void update(float timeStep);
    void updateImgui(ImGuiIO& io, float timeStep);

    inline constexpr Camera& getCamera() noexcept { return m_camera; }
};