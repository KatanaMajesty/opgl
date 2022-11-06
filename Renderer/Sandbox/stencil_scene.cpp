#include "Sandbox/stencil_scene.h"

#include "Common/window.h"

StencilScene::StencilScene(GLFWwindow* context) : Scene(context, "Stencil Test Scene")
    , m_camera(context)
{
    m_stencilShader = CreateShader("../Data/Shader/texture_default.vert", "../Data/Shader/outline.frag");
    Window::GetInstance()->AddCursorCallback([&](Window* window, double x, double y)
    {
        m_camera.UpdateEulers(x, y, window->IsCursorVisible() || !window->IsFocused());
    });

    Window::GetInstance()->AddScrollCallback([&](Window* window, double y)
    {
        m_camera.UpdateScroll(static_cast<float>(y));
    });
}

void StencilScene::Update(float timeStep)
{
    m_camera.UpdatePosition(timeStep);
}

void StencilScene::UpdateImgui(ImGuiIO& io, float timeStep)
{
}