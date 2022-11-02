#include "application.h"

Application* Application::s_application = nullptr;

Application::Application()
{
    Initialize();
    s_application = this;
}

Application::~Application()
{
    s_application = nullptr;
}

void Application::Initialize()
{
    m_window = std::make_unique<Window>(1920, 1080, "3D Renderer", false);
    
    std::cout << "Initializing application!\n";
}

void Application::Run()
{
    m_window->Open();
}