#include "Common/application.h"

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
    std::cout << "Initializing application!\n";

    if (m_window)
    {
        std::cout << "Found existing window... Closing and deleting it!\n";
        m_window->Close();
        m_window.release();
    }
    m_window = std::make_unique<Window>(1920, 1080, "3D Renderer", false);
}

void Application::Run()
{
    m_window->Open();
}