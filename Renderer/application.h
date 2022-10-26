#pragma once

#include "window.h"

class Application
{
private:
    Window m_window;

public:
    Application(int32_t width, int32_t height, const std::string& title, bool vsync = true);

    void run();
    
    inline Window& getWindow() { return m_window; }
};