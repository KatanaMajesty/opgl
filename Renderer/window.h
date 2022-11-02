#pragma once

#include "render_manager.h"

struct WindowInfo
{
    int32_t width;
    int32_t height;
    std::string title;
    bool vsync;
    bool cursorVisible;
    bool focused;
};

// Make window a singleton instead of RenderManager
class Window
{
public:
    using key_callback_type = std::function<void(Window*, float, int32_t)>;
    using scroll_callback_type = std::function<void(Window*, double)>;
    using cursor_callback_type = std::function<void(Window*, double, double)>;

public:
    Window(int32_t width, int32_t height, const std::string& title, bool vsync);
    ~Window();

    void Open();

    void Close();

    static inline Window* GetInstance() { return s_window; }

    static inline Window* OfContext(GLFWwindow* context) { return (Window*) glfwGetWindowUserPointer(context); }

    inline RenderManager& GetRenderManager() { return *m_renderManager.get(); }

    inline constexpr GLFWwindow* GetContext() { return m_context; }

    inline constexpr int32_t GetWidth() const { return m_info.width; }

    inline constexpr int32_t GetHeight() const { return m_info.height; }

    inline constexpr const std::string& GetTitle() const { return m_info.title; }

    inline constexpr bool IsVsync() const { return m_info.vsync; }

    inline constexpr bool IsCursorVisible() const { return m_info.cursorVisible; }

    inline void SetCursorVisible(bool visible) 
    { 
        m_info.cursorVisible = visible; 
        glfwSetInputMode(m_context, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED); 
    } 

    inline constexpr bool IsFocused() const { return m_info.focused; }

    inline void SetFocused(bool focused)
    {
        m_info.focused = true;
        glfwFocusWindow(m_context);
    }

    inline bool IsKeyPressed(int32_t key) const { return glfwGetKey(m_context, key); }

    template<typename T, typename... Args>
    T* AddScene(Args&&... args)
    {
        if (!m_renderManager)
            return nullptr;
            
        return GetRenderManager().AddScene<T>(m_context, std::forward<Args>(args)...);
    }

    void AddKeyCallback(int32_t key, key_callback_type callback);

    void AddScrollCallback(scroll_callback_type callback);

    void AddCursorCallback(cursor_callback_type callback);

private:
    void ProcessKeyboardInput(int32_t key, int32_t action);

    void ProcessScrollInput(double y);

    void ProcessCursorInput(double x, double y);

    void Update(float timeStep);

    void UpdateImgui(ImGuiIO& io, float timeStep);

private:
    static Window* s_window;

    WindowInfo m_info;
    GLFWwindow* m_context;
    std::unique_ptr<RenderManager> m_renderManager;
    std::vector<cursor_callback_type> m_cursorCallbacks;
    std::vector<scroll_callback_type> m_scrollCallbacks;
    std::unordered_map<int32_t, std::vector<key_callback_type>> m_keyboardCallbacks;
    float m_timeStep;
};