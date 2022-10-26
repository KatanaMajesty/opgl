#include "window.h"

Window::Window(int32_t width, int32_t height, const std::string& title, bool vsync)
    : m_width(width), m_height(height), m_title(title), m_vsync(vsync), m_context{nullptr}, m_renderManager{nullptr}
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_context = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_context)
    {
        std::cout << "Failed to create a window\n";
        return;
    }

    m_renderManager = std::make_unique<RenderManager>(m_context);

    glfwMakeContextCurrent(m_context);
	glfwSwapInterval(m_vsync);

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
	glfwSetInputMode(m_context, GLFW_CURSOR, m_cursorHidden ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    glfwSetFramebufferSizeCallback(m_context, [](GLFWwindow* context, int32_t width, int32_t height)
    {
        glViewport(0, 0, width, height);
    });

	glfwSetCursorPosCallback(m_context, [](GLFWwindow* context, double x, double y)
    {
        Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(context));
        window->getRenderManager().getCamera().processEulers(x, y, !window->m_cursorHidden);
    });

	glfwSetKeyCallback(m_context, [](GLFWwindow* context, int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(context));
        if (glfwGetKey(context, GLFW_KEY_F1) == GLFW_PRESS)
        {
            window->m_cursorHidden = !window->m_cursorHidden;
            glfwSetInputMode(context, GLFW_CURSOR, window->m_cursorHidden ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }
    });

	glfwSetScrollCallback(m_context, [](GLFWwindow* context, double x, double y)
    {
        Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(context));
        window->getRenderManager().getCamera().processScroll(static_cast<float>(y));
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

void Window::open()
{

    int glMaxAttribCount;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &glMaxAttribCount);
    std::cout << "Max vertex attrib count per vertex: " << glMaxAttribCount << std::endl;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(m_context))
    {
		float current = static_cast<float>(glfwGetTime());
		float timeStep = current - lastFrame;
		lastFrame = current;

        glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();

        update(timeStep);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        updateImgui(ImGui::GetIO(), timeStep);
        
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(m_context);
    }
}

void Window::update(float timeStep) { getRenderManager().update(timeStep); }

void Window::updateImgui(ImGuiIO& io, float timeStep) { getRenderManager().updateImgui(io, timeStep); }