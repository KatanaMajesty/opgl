#include "window.h"

Window* Window::s_window = nullptr;

Window::Window(int32_t width, int32_t height, const std::string& title, bool vsync)
    : m_info{
            .width = width, 
            .height = height, 
            .title = title, 
            .vsync = vsync, 
            .cursorVisible = false, 
            .focused = true
        }
    , m_renderManager{nullptr}
{
    if (s_window)
    {
        std::cout << "Window has already been created!\n";
        return;
    }
    s_window = this;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_context = glfwCreateWindow(m_info.width, m_info.height, GetTitle().c_str(), nullptr, nullptr);
    if (!m_context)
    {
        std::cout << "Failed to create a window\n";
        return;
    }

    // Window-related initialization stage
    m_renderManager = std::unique_ptr<RenderManager>(new RenderManager(m_context));

    glfwMakeContextCurrent(m_context);
	glfwSwapInterval(IsVsync());

	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_context, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW\n";
        return;
    }

    glfwSetWindowUserPointer(m_context, this);
	glfwSetInputMode(m_context, GLFW_CURSOR, IsCursorVisible() ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

    // Setup window callbacks, redirecting them to our own callbacks
    glfwSetWindowSizeCallback(m_context, [](GLFWwindow* context, int32_t width, int32_t height)
    {
        Window* window = Window::OfContext(context);
        window->m_info.width = width;
        window->m_info.height = height;
    });

    glfwSetFramebufferSizeCallback(m_context, [](GLFWwindow* context, int32_t width, int32_t height)
    {
        glViewport(0, 0, width, height);
    });

	glfwSetCursorPosCallback(m_context, [](GLFWwindow* context, double x, double y)
    {
        Window* window = Window::OfContext(context);
        window->ProcessCursorInput(x, y);
    });

	glfwSetScrollCallback(m_context, [](GLFWwindow* context, double x, double y)
    {
        Window* window = Window::OfContext(context);
        window->ProcessScrollInput(y);
    });

    glfwSetKeyCallback(m_context, [](GLFWwindow* context, int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        Window* window = Window::OfContext(context);
        window->ProcessKeyboardInput(key, action);
    });

    glfwSetWindowFocusCallback(m_context, [](GLFWwindow* context, int32_t focused)
    {
        Window* window = Window::OfContext(context); 
        window->m_info.focused = (bool) focused;
    });
}

Window::~Window()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_context);
    glfwTerminate();
}

void Window::Open()
{
    int glMaxAttribCount;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &glMaxAttribCount);
    std::cout << "Max vertex attrib count per vertex: " << glMaxAttribCount << std::endl;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
    
    glEnable(GL_DEPTH_TEST); 
    glDepthMask(GL_TRUE); // enable writing

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(m_context))
    {
		float current = static_cast<float>(glfwGetTime());
		m_timeStep = current - lastFrame;
		lastFrame = current;

        glClearColor(0.14f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glfwPollEvents();

        Update(m_timeStep);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        UpdateImgui(ImGui::GetIO(), m_timeStep);
        
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(m_context);
    }
}

void Window::Close()
{
    glfwSetWindowShouldClose(m_context, true);
}

void Window::AddKeyCallback(int32_t key, key_callback_type callback)
{
    m_keyboardCallbacks[key].push_back(callback);    
}

void Window::AddScrollCallback(scroll_callback_type callback)
{
    m_scrollCallbacks.push_back(callback);
}

void Window::AddCursorCallback(cursor_callback_type callback)
{
    m_cursorCallbacks.push_back(callback);
}

void Window::Update(float timeStep) 
{ 
    GetRenderManager().Update(timeStep); 
}

void Window::UpdateImgui(ImGuiIO& io, float timeStep) 
{ 
    GetRenderManager().UpdateImgui(io, timeStep); 
}

void Window::ProcessKeyboardInput(int32_t key, int32_t action)
{
    auto it = m_keyboardCallbacks.find(key);
    if (it == m_keyboardCallbacks.end())
        return;

    for (key_callback_type& callback : it->second)
        std::invoke(callback, this, m_timeStep, action);
}

void Window::ProcessScrollInput(double y)
{
    for (scroll_callback_type& callback : m_scrollCallbacks)
        std::invoke(callback, this, y);
}

void Window::ProcessCursorInput(double x, double y)
{
    for (cursor_callback_type& callback : m_cursorCallbacks)
        std::invoke(callback, this, x, y);
}