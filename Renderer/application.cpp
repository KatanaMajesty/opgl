#include "application.h"

Application::Application(int32_t width, int32_t height, const std::string& title, bool vsync)
    : m_window(width, height, title, vsync)
{
}

void Application::run()
{
    m_window.open();
}