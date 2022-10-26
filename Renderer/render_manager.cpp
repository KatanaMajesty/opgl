#include "render_manager.h"

#include "window.h"
#include "camera.h"

void processInput(GLFWwindow *context, float delta)
{
        Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(context));
        if (glfwGetKey(context, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(context, true);

        Camera& cam = window->getRenderManager().getCamera();
        // rewrite in future
        if (glfwGetKey(context, GLFW_KEY_W) == GLFW_PRESS)
                cam.processMovement(GLFW_KEY_W, delta);

        if (glfwGetKey(context, GLFW_KEY_S) == GLFW_PRESS)
                cam.processMovement(GLFW_KEY_S, delta);

        if (glfwGetKey(context, GLFW_KEY_A) == GLFW_PRESS)
                cam.processMovement(GLFW_KEY_A, delta);

        if (glfwGetKey(context, GLFW_KEY_D) == GLFW_PRESS)
                cam.processMovement(GLFW_KEY_D, delta);
}

RenderManager::RenderManager(GLFWwindow* context)
    : m_context(context), m_camera(context)
{
}

void RenderManager::update(float timeStep)
{
        processInput(m_context, timeStep);
        for (auto& scene : m_scenes)
                scene->update(timeStep);
}

void RenderManager::updateImgui(ImGuiIO& io, float timeStep)
{
        ImGui::Begin("Interaction Window!");

        ImGui::Text("Current FPS on Scene: %.2f", io.Framerate);

        ImGui::End();
}