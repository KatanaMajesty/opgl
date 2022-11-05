#include "Common/application.h"
#include "Common/window.h"

#include "Sandbox/model_scene.h"

/*
TODO:
- Use unordered_map instead of vector in texture caching
*/

int main() 
{
    std::unique_ptr<Application> application = std::make_unique<Application>();
    Window::GetInstance()->AddScene<ModelScene>();
    // Window::GetInstance()->AddScene<BlendingScene>();
    application->Run();
}
