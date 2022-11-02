#pragma once

#include "window.h"

class Application
{
public:
    Application();
    virtual ~Application();

    static inline Application* GetInstance() { return s_application; }

    static inline bool IsKeyPressed(int32_t key) { return GetInstance()->m_window->IsKeyPressed(key); }

protected:
    virtual void Initialize();

private:
    void Run();

private:
    friend int main();

    static Application* s_application;
    std::unique_ptr<Window> m_window;
};