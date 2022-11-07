#include "Common/application.h"
#include "Common/window.h"

#include "Sandbox/model_scene.h"
#include "Sandbox/stencil_scene.h"
#include "Sandbox/blend_scene.h"

int main() 
{
    std::unique_ptr<Application> application = std::make_unique<Application>();
    Window& window = application->GetWindow();
    window.RegisterScene<ModelScene>();
    window.RegisterScene<StencilScene>();
    window.RegisterScene<BlendScene>();
    window.AddKeyCallback(GLFW_KEY_F1, [](Window* window, float timeStep, int32_t action)
    {
        if (action == GLFW_PRESS)
            window->SetCursorVisible(!window->IsCursorVisible());
    });
    window.AddKeyCallback(GLFW_KEY_F2, [](Window* window, float timeStep, int32_t action)
    {
        // go back to menu
        if (action == GLFW_PRESS)
            window->GetRenderManager().SetMenuEnabled(!window->GetRenderManager().IsMenuEnabled());
    });
    window.AddKeyCallback(GLFW_KEY_ESCAPE, [](Window* window, float timeStep, int32_t action) { window->Close(); });
    application->Run();
}
