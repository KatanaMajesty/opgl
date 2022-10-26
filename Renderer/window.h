#pragma once

#include "render_manager.h"

class Window
{
private:
    int32_t m_width;
    int32_t m_height;
    std::string m_title;
    bool m_vsync;
    bool m_cursorHidden = true;
    GLFWwindow* m_context;
    std::unique_ptr<RenderManager> m_renderManager;
    glm::vec3 m_clearColor = glm::vec3(0.14f, 0.15f, 0.15f);

public:
    Window(int32_t width, int32_t height, const std::string& title, bool vsync);
    ~Window();

    void open();

    inline RenderManager& getRenderManager() { return *m_renderManager.get(); }
    inline GLFWwindow* getContext() { return m_context; }
    inline void setClearColor(glm::vec3 clearColor) { m_clearColor = clearColor; }

private:
    void update(float timeStep);
    void updateImgui(ImGuiIO& io, float timeStep);
};